#include <stdio.h>
#include <string>
#include <vector>

namespace MessagingSystem {

	class IMessageHandler;


	struct MessageDetails {};


	class IMessageHandler {

	public:

		virtual void HandleMessage(const std::size_t & i_Message, MessageDetails * i_pDetails) = 0;
	};

	struct MessageInfo {

		IMessageHandler * handler;
		std::size_t message;

	};

	std::vector<MessageInfo> HandlerRegistry = std::vector<MessageInfo>();

	void DisposeMessagingSystem() {

		HandlerRegistry.~vector();
	}

	size_t HandlerCount() {

		return HandlerRegistry.size();
	}

	// Declare a type for a Message Handler
	typedef void(*MessageHandler_t)(const std::size_t & i_Message);
	void RegisterMessageHandler(const std::size_t & i_Message, IMessageHandler * i_pHandler) {

		//add to aggregate list
		MessageInfo info = MessageInfo();
		info.handler = i_pHandler;
		info.message = i_Message;
		HandlerRegistry.push_back(info);

		//printf("Message Handler Registered - %zx\n", i_Message);
		//printf("Counting %d handlers total\n", HandlerRegistry.size());
	}

	void DeregisterMessageHandler(const std::size_t & i_Message, IMessageHandler * i_pHandler) {

		for (size_t i = 0; i < HandlerRegistry.size(); i++) {

			if (HandlerRegistry[i].handler == i_pHandler && HandlerRegistry[i].message == i_Message) {

				HandlerRegistry.erase(HandlerRegistry.begin() + i);

				//printf("Message Handler Deregistered - %zx\n", i_Message);
				//printf("Counting %d handlers total\n", HandlerRegistry.size());
				return;
			}
		}

	}

	void SendEngineMessage(const std::size_t & i_Message, MessageDetails * i_pMessageDetails) {

		int count = 0;

		for (size_t i = 0; i < HandlerRegistry.size(); i++) {

			if (HandlerRegistry[i].message == i_Message) {

				HandlerRegistry[i].handler->HandleMessage(i_Message, i_pMessageDetails);

				count++;
			}
		}

		printf("Sent message - %zu to %d handlers\n", i_Message, count);

	}
}

