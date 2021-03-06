#include "pch.h"
#include <iostream>
#include "Source/Engine/MessagingSystem.h"
#include <string>

const std::string testMessage = "Test Message";

struct TestDetails : public MessagingSystem::MessageDetails {

	float testData;

};

class TestHandler : public MessagingSystem::IMessageHandler {

public:


	void HandleMessage(const std::size_t & i_Message, MessagingSystem::MessageDetails * details) override {


		if (i_Message == std::hash<std::string>{}(testMessage)) {

			printf("Handling Message - %zx	", i_Message);
			printf("Unpacking message details:	");
			printf("Test data: %f\n", reinterpret_cast <TestDetails *>(details)->testData);

		}
	}

};

int main()
{
	const int maxMessages = 100;
	float count = 0;
	std::size_t messageHash = std::hash<std::string>{}(testMessage);

	TestHandler * handlers[maxMessages];

	for (int i = 0; i < maxMessages; i++) {

		TestHandler * testHandler = new TestHandler(); //create a handler
		RegisterMessageHandler(messageHash, testHandler); //register the handler
		handlers[i] = testHandler;

	}

	printf("%d handlers registered\n\n", MessagingSystem::HandlerCount());

	TestDetails * details = new TestDetails();
	details->testData = 5.0;

	MessagingSystem::SendEngineMessage(messageHash, details);


	for (int i = 0; i < maxMessages; i++) {

		MessagingSystem::DeregisterMessageHandler(messageHash, handlers[i]);

	}

	printf("\n%d handlers registered.\n", MessagingSystem::HandlerCount());

	system("pause");
}
