all:
	g++ -g -Wall Network.cpp CanNetwork.cpp main.cpp -o CamManager -lws2_32


run:
	./ChatClient.exe
	
buildRun:
	g++ -g -Wall ../Network/Network.cpp -I"../Network/" ../Chat/Chat.cpp -I"../Chat/" ChatClient.cpp -o ChatClient -lws2_32
	./ChatClient.exe