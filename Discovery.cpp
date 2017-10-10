#include "Discovery.hpp"
#include "Message.hpp"
#include "UDPService.hpp"

void Discovery::sendHello(string host, string port) {

	UDPClient udpClient;

	DiscoveryMessage helloMessage = DiscoveryMessage(this->username);

	udpClient.sendMessage(host, port, helloMessage.getMessageBody());

}