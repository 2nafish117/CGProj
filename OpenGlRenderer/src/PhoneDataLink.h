#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

// windows defines min and max as macros, we do not want them
#define NOMINMAX
#include <stdio.h>
#include <winsock2.h>

#include <glm/glm.hpp>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

// uncomment to connect to the phone
//#define PHONE_LINK

// OLD FORMAT: 2843298.60868, 3,   0.915,  5.765,  7.887, 4,  -0.043,  0.045,  0.000 
// NEW FORMAT: vec3 : gameRotation, vec3 : gravity, vec3 : linearAcceleration

struct PhoneSensorData
{
	typedef glm::vec3 Rotation;
	typedef glm::vec3 Gravity;
	typedef glm::vec3 Accel;
	Rotation rot;
	Gravity grav;
	Accel accel;
};


class PhoneDataLink
{
public:
	PhoneDataLink()
	{
#ifdef PHONE_LINK

		mBuf = new char[BUFF_LEN];
		mLen = 0;

		//Initialise winsock
		//printf("\nInitialising Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &mWsa) != 0)
		{
			printf("Failed.Error Code : %d", WSAGetLastError());
			__debugbreak();
			exit(EXIT_FAILURE);
		}
		//printf("Initialised.\n");

		//Create a socket
		if ((mSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			printf("Could not create socket : %d", WSAGetLastError());
			__debugbreak();
		}
		//printf("Socket created.\n");

		//Prepare the sockaddr_in structure
		mAddr.sin_family = AF_INET;
		mAddr.sin_addr.s_addr = INADDR_ANY;
		mAddr.sin_port = htons(5555);

		//Bind
		if (bind(mSocket, (struct sockaddr *)& mAddr, sizeof(mAddr)) == SOCKET_ERROR)
		{
			printf(" Bind failed with error code : %d", WSAGetLastError());
			__debugbreak();
			exit(EXIT_FAILURE);
		}
		//puts(" Bind done");

#endif // DEBUG
	}

	~PhoneDataLink()
	{
#ifdef PHONE_LINK
		delete[] mBuf;
		closesocket(mSocket);
		WSACleanup();
#endif
	}

	void recvPhonePosData(PhoneSensorData* oPhone)
	{
#ifdef PHONE_LINK
		//clear the buffer by filling null, it might have previously received data
		memset(mBuf, NULL, BUFF_LEN);

		int size = sizeof(mPhoneAddr);
		//try to receive some data, this is a blocking call
		if ((mLen = recvfrom(mSocket, mBuf, BUFF_LEN, 0, (struct sockaddr *) &mPhoneAddr, &size)) == SOCKET_ERROR)
		{
			printf(" recvfrom() failed with error code : %d", WSAGetLastError());
			__debugbreak();
			exit(EXIT_FAILURE);
		}

		sscanf_s(mBuf, "%f,%f,%f, %f,%f,%f, %f,%f,%f",
			&oPhone->rot.x, &oPhone->rot.y, &oPhone->rot.z,
			&oPhone->grav.x, &oPhone->grav.y, &oPhone->grav.z,
			&oPhone->accel.x, &oPhone->accel.y, &oPhone->accel.z);
#else
		oPhone->accel = {};
		oPhone->grav = {};
		oPhone->rot = {};
#endif

		return;
	}


private:
	char* mBuf;
	int mLen;
	SOCKET mSocket;
	sockaddr_in mAddr, mPhoneAddr;
	WSADATA mWsa;
	static constexpr int BUFF_LEN = 128;
};

# if 0
// example usage!!
int main()
{
	PhoneDataLink link;
	PhoneSensorData phone;
	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 velocity = { 0, 0, 0 };
	glm::vec3 rotation = { 0, 0, 0 };
	float dt = 200.0 / 1000.0; // milliseconds
	while (true)
	{
		link.recvPhonePosData(&phone);
		/*
		printf("Recvd: %f, %d, %f, %f, %f, %d, %f, %f, %f\n\n",
			phone.unknown_field0,
			phone.unknown_field1,
			phone.accel.x, phone.accel.y, phone.accel.z,
			phone.unknown_field2,
			phone.gyro.x, phone.gyro.y, phone.gyro.z);
		*/

		velocity.x = phone.accel.x * dt;
		velocity.y = phone.accel.y * dt;
		velocity.z = phone.accel.z * dt;

		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
		position.z += velocity.z * dt;

		//position.x += 0.5 * phone.accel.x * dt * dt;
		//position.y += 0.5 * phone.accel.y * dt * dt;
		//position.z += 0.5 * phone.accel.z * dt * dt;

		//printf("position: %f %f %f\n", position.x, position.y, position.z);
		printf("roatation: %f %f %f\n", phone.rot.x, phone.rot.y, phone.rot.z);
		//printf("velocity: %f %f %f\n", velocity.x, velocity.y, velocity.z);

	}
	return 0;
}
# endif