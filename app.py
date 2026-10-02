import asyncio
import nest_asyncio
from pydantic import create_model
from mcp import ClientSession, StdioServerParameters
from mcp.client.stdio import stdio_client
from langchain_ollama import ChatOllama
from langchain_core.tools import StructuredTool
from langchain_core.messages import HumanMessage, AIMessage
from langchain_classic.agents import AgentExecutor, create_tool_calling_agent
from langchain_core.prompts import ChatPromptTemplate, MessagesPlaceholder

nest_asyncio.apply()

async def main():
    # 1. 設定 MCP Server 參數
    server_params = StdioServerParameters(
        command="./build/QtLegacyProject_MCP.exe", # 請確認路徑是否正確
        args=[]
    )

    print("[系統] 正在啟動 C++ MCP Server...")
    
    # 使用 async context manager 確保在整個聊天過程中連線保持開啟
    async with stdio_client(server_params) as (read, write):
        async with ClientSession(read, write) as session:
            await session.initialize()
            print("[系統] MCP Server 初始化完成！")
            
            # 2. 獲取並註冊工具
            tools_result = await session.list_tools()
            print(f"[系統] 成功載入 {len(tools_result.tools)} 個工具")
            
            lc_tools = []
            for mcp_tool in tools_result.tools:
                # 新增 input_schema 參數
                def create_lc_tool(tool_name, tool_desc, input_schema):
                    
                    # --- 1. 解析 MCP 的 JSON Schema ---
                    properties = input_schema.get("properties", {})
                    required_fields = input_schema.get("required", [])
                    
                    # --- 2. 動態建立 Pydantic 欄位定義 ---
                    fields = {}
                    for key, val in properties.items():
                        # 簡單對應型別
                        field_type = str
                        if val.get("type") == "boolean":
                            field_type = bool
                        elif val.get("type") == "integer":
                            field_type = int
                            
                        # 判斷是否為必填欄位
                        if key in required_fields:
                            fields[key] = (field_type, ...)
                        else:
                            fields[key] = (field_type, None)

                    # --- 3. 生成 DynamicSchema ---
                    DynamicSchema = create_model(f"{tool_name}_Schema", **fields)

                    async def async_tool_executor(**kwargs):
                        print(f"\n   ⚙️ [執行工具] {tool_name} ...")
                        result = await session.call_tool(tool_name, arguments=kwargs)
                        text_results = [c.text for c in result.content if c.type == "text"]
                        return "\n".join(text_results)

                    return StructuredTool.from_function(
                        coroutine=async_tool_executor,
                        name=tool_name,
                        description=tool_desc,
                        args_schema=DynamicSchema # 注入 Schema，防止參數被丟棄
                    )
                
                # 呼叫時傳入 mcp_tool.inputSchema
                lc_tools.append(create_lc_tool(mcp_tool.name, mcp_tool.description, mcp_tool.inputSchema))
                print(f"       - {mcp_tool.name}")

            # 3. 初始化 LLM 與 Agent
            llm = ChatOllama(model="qwen3", temperature=0)
            
            # 建立 Prompt 模板，包含對話歷史與 Agent 所需的草稿區 (agent_scratchpad)
            prompt = ChatPromptTemplate.from_messages([
                ("system", "你是一個協助操作與測試系統的 AI 助手。請根據使用者的指示，適當地呼叫工具來完成任務，並簡明扼要地回報結果。"),
                MessagesPlaceholder(variable_name="chat_history"),
                ("human", "{input}"),
                MessagesPlaceholder(variable_name="agent_scratchpad"),
            ])

            # 建立 Tool Calling Agent
            agent = create_tool_calling_agent(llm, lc_tools, prompt)
            
            # 建立 AgentExecutor 負責自動處理多步工具呼叫的迴圈
            # 設定 verbose=True 可以看到 Agent 內部的推理與工具呼叫過程
            agent_executor = AgentExecutor(agent=agent, tools=lc_tools, verbose=True)
            
            chat_history = []

            print("\n==================================================")
            print("🤖 系統測試機器人已上線 (Agent 模式)！(輸入 'exit', 'quit' 或 'q' 離開)")
            print("==================================================")

            # 4. 建立互動式對話迴圈
            while True:
                try:
                    user_input = input("\n👤 你: ")
                    if user_input.strip().lower() in ['exit', 'quit', 'q']:
                        print("👋 結束對話，關閉 MCP 伺服器。")
                        break
                    
                    if not user_input.strip():
                        continue

                    print("🤖 思考與執行中...")
                    
                    # 執行 AgentExecutor，傳入使用者輸入與對話歷史
                    response = await agent_executor.ainvoke({
                        "input": user_input,
                        "chat_history": chat_history
                    })
                    
                    final_answer = response["output"]
                    print(f"\n🤖 AI: {final_answer}")
                    
                    # 紀錄對話歷史，供下一輪對話使用
                    chat_history.append(HumanMessage(content=user_input))
                    chat_history.append(AIMessage(content=final_answer))

                except KeyboardInterrupt:
                    print("\n👋 收到中斷訊號，關閉 MCP 伺服器。")
                    break
                except Exception as e:
                    print(f"\n❌ 發生錯誤: {e}")

if __name__ == "__main__":
    asyncio.run(main())