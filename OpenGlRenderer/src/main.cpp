#include "OpenGLDependencies.h"
#include "Util.h"
#include "Renderer.h"
#include "Geometry.h"
#include "Camera.h"

#include "Lighting.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "PhoneDataLink.h"

// forward declaration, definitions at end of main.cpp
static void showDirLightControls(DirLight& dirLight);
static void showPointLightControls(PointLight& pointLight, int index = 0);
static void showSpotLightControls(SpotLight& spotLight, int index = 0);

static void ShowDemoWindowLayout();

int main(void)
{
	unsigned int mScreenWidth = 1280;//960;
	unsigned int mScreenHeight = 720;//540;
	GLFWwindow* mWindow;
	std::string mGameName = "Untitled Game Game";

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cerr << "glfwInit failed\n";
		return EXIT_FAILURE;
	}

	/* Create a windowed mode window and its OpenGL context */
	mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mGameName.c_str(), NULL, NULL);
	if (!mWindow)
	{
		std::cerr << "glfwCreateWindow failed\n";
		return EXIT_FAILURE;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(mWindow);

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLenum err = glewInit();
	if (err != 0)
	{
		std::cerr << "glewInit failed with error code " << err << "\n";
		return EXIT_FAILURE;
	}
	// start of empty scope
	{
#include "vertexData.h"


		const int numCubes = 6;
		std::vector<glm::vec3> cubes;
		cubes.reserve(2 * numCubes * numCubes);
		//for (int j = 0; j < numCubes; ++j)
		//	for (int i = 0; i < numCubes; ++i)
		//		cubes.push_back({ 2 * i - 10, 2 * j - 2, -5 });
		
		for (int j = 0; j < numCubes; ++j)
			for (int i = 0; i < numCubes; ++i)
				cubes.push_back({ 2 * i - 5, 0 , 2 * j - 5});

		//for (int i = 0; i < numCubes * numCubes; ++i)
		//	cubes.push_back(
		//		glm::vec3(
		//		utl::random(-20, 20),
		//		utl::random(-20, 20),
		//		utl::random(-20, 20))
		//	);

		Renderer mRenderer;
		mRenderer.setClearColor(0.1, 0.1, 0.1, 1.0);
		mRenderer.enable(GL_DEPTH_TEST);
		mRenderer.enable(GL_BLEND);
		mRenderer.setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 330 core");
		ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
		ImGui::StyleColorsDark();

		VertexArray cubeVA;
		cubeVA.bind();
		VertexBuffer cubeVB(cubeVertex, 24 * 8 * sizeof(float));
		IndexBuffer cubeIB(cubeIndex, 6 * 6);
		VertexBufferLayout cubeVBLayout;
		cubeVBLayout.push<float>(3);	// vertex data
		cubeVBLayout.push<float>(3);	// normal data
		cubeVBLayout.push<float>(2);	// texture data
		cubeVA.addBuffer(cubeVB, cubeVBLayout);
		ShaderProgram cubeShader("res/shaders/cubeVertexShader.vert", "res/shaders/cubeFragmentShader.frag");
		Texture woodDiffTexture("res/textures/wood_crate_texture.png");
		Texture woodSpecTexture("res/textures/wood_crate_texture_specular_map.png");

		woodDiffTexture.bind(0);
		woodSpecTexture.bind(1);

		cubeShader.bind();
		cubeShader.seti("uMaterial.diffuse", 0);
		cubeShader.seti("uMaterial.specular", 1);

		VertexArray lightVA;
		lightVA.bind();
		VertexBuffer lightVB(lightVertex, 24 * 3 * sizeof(float));
		IndexBuffer lightIB(lightIndex, 6 * 6);
		VertexBufferLayout lightVBLayout;
		lightVBLayout.push<float>(3);	// vertex data
		lightVA.addBuffer(lightVB, lightVBLayout);
		ShaderProgram lightShader("res/shaders/lightVertexShader.vert", "res/shaders/lightFragmentShader.frag");

		//VertexArray circleVA;
		//circleVA.bind();
		//VertexBuffer circleVB(circleVertex, 64 * 3 * sizeof(float));
		//IndexBuffer circleIB(circleIndex, 189);
		//VertexBufferLayout circleVBLayout;
		//circleVBLayout.push<float>(3);	// vertex data
		//circleVA.addBuffer(circleVB, circleVBLayout);
		// ShaderProgram cirShader("res/shaders/lightVertexShader.vert", "res/shaders/lightFragmentShader.frag");

		VertexArray cylinderVA;
		cylinderVA.bind();
		VertexBuffer cylinderVB(cylinderVertex, 128 * 6 * sizeof(float));
		IndexBuffer cylinderIB(cylinderIndex, 252);
		VertexBufferLayout cylinderVBLayout;
		cylinderVBLayout.push<float>(3);	// vertex data
		cylinderVBLayout.push<float>(3);	// normal data
		cylinderVA.addBuffer(cylinderVB, cylinderVBLayout);
		ShaderProgram hardLitSurfaceShader("res/shaders/hardLitSurface.vert", "res/shaders/hardLitSurface.frag");

		VertexArray cyberTruckVA;
		cyberTruckVA.bind();
		VertexBuffer cyberTruckVB(cyberTruckVertex, 78 * 6 * sizeof(float));
		IndexBuffer cyberTruckIB(cyberTruckIndex, 132);
		VertexBufferLayout cyberTruckVBLayout;
		cyberTruckVBLayout.push<float>(3);	// vertex data
		cyberTruckVBLayout.push<float>(3);	// normal data
		cyberTruckVA.addBuffer(cyberTruckVB, cyberTruckVBLayout);

		// Camera settings
		//const glm::vec3 pos = glm::vec3(-60.0f, 5.0f, 5.0f);
		const glm::vec3 pos = glm::vec3(0.0f, 0.0f, 20.0f);
		const glm::vec3 dir = glm::vec3(0.0f, 0.0f, 1.0f);
		// pos: -61.1335 5.36623 5.08433
		// dir: 0.991833 -0.0654031 -0.109499
		Camera camera(pos, dir);
		auto scrollCallbackArgs = std::make_tuple(&camera, 0.0);

		// starts listenig to udp 192.168.0.100:5555
		PhoneDataLink link;
		PhoneSensorData phone;

		link.recvPhonePosData(&phone);
		glm::mat4 calRot = glm::rotate(glm::mat4(1), phone.rot.x, glm::vec3(1, 0, 0));
		calRot = glm::rotate(calRot, phone.rot.y, glm::vec3(0, 1, 0));
		calRot = glm::rotate(calRot, -phone.rot.z, glm::vec3(0, 0, 1));

		glfwSetWindowUserPointer(mWindow, &scrollCallbackArgs);
		glfwSetScrollCallback(mWindow, Camera::handleMouseScrollEvent);
		glfwSetCursorPos(mWindow, 0, 0);

		glm::mat4 viewProjection;
		glm::mat4 model;
		glm::mat3 normal;

		// lights
		DirLight dirLight;

		const int numPtLight = 2;
		PointLight pointLight[numPtLight] = {
			PointLight(glm::vec3(0, 0, 5)),
			PointLight(glm::vec3(5, -5, -5))
		};

		//for (int i = 0; i < numPtLight; ++i)
		//	pointLight[i] = PointLight(glm::vec3(
		//		utl::random(-20, 20),
		//		utl::random(-20, 20),
		//		utl::random(-20, 20)
		//	));

		const int numSptLight = 1;
		SpotLight spotLight[numSptLight] = {
			SpotLight(glm::vec3(0, 0, -10))
		};

		float materialShininess = 32.0f;

		float deltaTime = 0;
		float timeLastFrame = 0;
		float timeThisFrame = 0;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(mWindow))
		{
			mRenderer.clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			timeThisFrame = (float)glfwGetTime();
			deltaTime = timeThisFrame - timeLastFrame;
			timeLastFrame = timeThisFrame;

			if(glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
			{
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				camera.mSensitivity = SENSITIVITY;

				std::get<1>(scrollCallbackArgs) = deltaTime;
				// handle inputs
				if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
					glfwSetWindowShouldClose(mWindow, true);

				if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
					camera.handleKeyboardInput(FRONT, true, deltaTime);
				if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
					camera.handleKeyboardInput(BACK, true, deltaTime);
				if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
					camera.handleKeyboardInput(LEFT, true, deltaTime);
				if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
					camera.handleKeyboardInput(RIGHT, true, deltaTime);
				if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
					camera.handleKeyboardInput(SPRINT, true, deltaTime);
				if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
					camera.handleKeyboardInput(WALK, true, deltaTime);

				double mouseXpos = 0, mouseYpos = 0 ;
				glfwGetCursorPos(mWindow, &mouseXpos, &mouseYpos);
				camera.handleMouseMoveInput(mouseXpos, mouseYpos, deltaTime);
				int leftButtonPressed = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT);
				int rightButtonPressed = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT);
				int middleButtonPressed = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT);
				camera.handleMouseButtonInput(leftButtonPressed, rightButtonPressed, middleButtonPressed, deltaTime);

			}
			else {
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				camera.mSensitivity = 0.0f;
			}

			{
				// Phone Stuff
				// recv data
				link.recvPhonePosData(&phone);

				float speed = 50.0f;
				//camera.mPos += speed * phone.accel * deltaTime * deltaTime; // position integratkion code

			}

			{
				// render stuff
				viewProjection = camera.getViewProjectionMatrix();

				// textured cubes
				cubeShader.bind();
				cubeShader.setDirLight("uDirLight", dirLight);

				for (int i = 0; i < numPtLight; ++i)
					cubeShader.setPointLight("uPointLight[" + std::to_string(i) + "]", pointLight[i]);

				for (int i = 0; i < numSptLight; ++i)
					cubeShader.setSpotLight("uSpotLight[" + std::to_string(i) + "]", spotLight[i]);

				cubeShader.setf("uMaterial.shininess", materialShininess);
				cubeShader.set3fv("uCameraPos", glm::value_ptr(camera.mPos));

				for (int i = 0; i < cubes.size(); ++i)
				{
					glm::mat4 leftToRight(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, -1.0f, 0.0f, 
						0.0f, 1.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f, 1.0f );
					glm::quat rotationQuat(phone.rot);
					//glm::normalize(rotationQuat);
					glm::mat4 rotation = glm::toMat4(rotationQuat);

					model = leftToRight * rotation * rotation;
					model = glm::translate(model, cubes[i]);
					
					//model = glm::translate(glm::mat4(1), cubes[i]);
					normal = glm::mat3(glm::transpose(glm::inverse(model)));
					cubeShader.setMatrix4fv("uModel", glm::value_ptr(model));
					cubeShader.setMatrix3fv("uNormal", glm::value_ptr(normal));
					cubeShader.setMatrix4fv("uViewProjection", glm::value_ptr(viewProjection));
					mRenderer.draw(cubeVA, cubeIB, cubeShader);
				}

				// hard lit surfaces 
				hardLitSurfaceShader.bind();
				hardLitSurfaceShader.setMatrix4fv("uViewProjection", glm::value_ptr(viewProjection));

				hardLitSurfaceShader.setDirLight("uDirLight", dirLight);

				for (int i = 0; i < numPtLight; ++i)
					hardLitSurfaceShader.setPointLight("uPointLight[" + std::to_string(i) + "]", pointLight[i]);

				for (int i = 0; i < numSptLight; ++i)
					hardLitSurfaceShader.setSpotLight("uSpotLight[" + std::to_string(i) + "]", spotLight[i]);

				hardLitSurfaceShader.set3fv("uCameraPos", glm::value_ptr(camera.mPos));
				hardLitSurfaceShader.setf("uMaterial.shininess", materialShininess);

				hardLitSurfaceShader.set3f("uMaterial.diffuse", 0.05, 0.05, 0.05);
				hardLitSurfaceShader.set3f("uMaterial.specular", 0.1, 0.1, 0.1);

				// front wheel
				glm::vec3 wheelScale = glm::vec3(0.7, 0.7, 0.2);
				glm::vec3 wheelPos[4] = {
					// front
					glm::vec3(0.85, 0.1,  0.75),
					glm::vec3(0.85, 0.1, -0.75),

					// back
					glm::vec3(3.75, 0.1,  0.75),
					glm::vec3(3.75, 0.1, -0.75),
				};
				glm::vec3 carPos(0, 0, 0);
				//carPos.x = 10 * sin(5 * timeThisFrame);
				//carPos.y = 10 * cos(5 * timeThisFrame);
				//carPos.x = sin(timeThisFrame);

				for (int i = 0; i < 4; ++i)
				{

					model = glm::translate(glm::mat4(1.0), wheelPos[i] + carPos);
					model = glm::scale(model, wheelScale);
					hardLitSurfaceShader.setMatrix4fv("uModel", glm::value_ptr(model));
					//mRenderer.draw(cylinderVA, cylinderIB, hardLitSurfaceShader);
				}

				model = glm::mat4(1.0f);
				
				model = glm::translate(model, carPos);
				hardLitSurfaceShader.setMatrix4fv("uModel", glm::value_ptr(model));
				hardLitSurfaceShader.set3f("uMaterial.diffuse", 0.7, 0.7, 0.7);
				hardLitSurfaceShader.set3f("uMaterial.specular", 0.9, 0.9, 0.9);

				//mRenderer.draw(cyberTruckVA, cyberTruckIB, hardLitSurfaceShader);

				// light boxes
				lightShader.bind();
				
				lightShader.setMatrix4fv("uViewProjection", glm::value_ptr(viewProjection));
				for (int i = 0; i < numPtLight; ++i)
				{
					model = glm::translate(glm::mat4(1.0), pointLight[i].mPosition);
					lightShader.setMatrix4fv("uModel", glm::value_ptr(model));
					//mRenderer.draw(lightVA, lightIB, lightShader);
				}

				for (int i = 0; i < numSptLight; ++i)
				{
					model = glm::translate(glm::mat4(1.0), spotLight[i].mPosition);
					lightShader.setMatrix4fv("uModel", glm::value_ptr(model));
					//mRenderer.draw(lightVA, lightIB, lightShader);
				}

				// Imgui stuff
				{
					//ShowDemoWindowLayout();

					ImGui::Begin("Editor");
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

					showDirLightControls(dirLight);
					for (int i = 0; i < numPtLight; ++i)
						showPointLightControls(pointLight[i], i);
					for (int i = 0; i < numSptLight; ++i)
						showSpotLightControls(spotLight[i], i);

					if (ImGui::TreeNode("Box Material"))
					{
						ImGui::BeginGroup();
						ImGui::SliderFloat("Shininess", &materialShininess, 1, 128);
						ImGui::EndGroup();
						ImGui::TreePop();
					}

					ImGui::End();
				}
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(mWindow);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mWindow);
	glfwTerminate();

	return 0;
}

static void showDirLightControls(DirLight& light)
{
	if (ImGui::TreeNode("Directional Light"))
	{
		ImGui::BeginGroup();
		ImGui::InputFloat3("Direction", glm::value_ptr(light.mDirection), 2);
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("Properties", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Ambient"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mAmbient));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Diffuse"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mDiffuse));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Specular"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mSpecular));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndGroup();
		ImGui::TreePop();
	}
}

static void showPointLightControls(PointLight& light, int index)
{
	ImGui::PushID(index);
	if (ImGui::TreeNode("Point Light"))
	{
		ImGui::BeginGroup();
		ImGui::SliderFloat3("Pos", glm::value_ptr(light.mPosition), -15.0f, 15.0f);
		ImGui::SliderFloat("mKConstant", &light.mKConstant, 1.0, 1.0);
		ImGui::SliderFloat("mKLinear", &light.mKLinear, 0.0f, 1.0f);
		ImGui::SliderFloat("mKQuadratic", &light.mKQuadratic, 0.0f, 0.1f);
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("Properties", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Ambient"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mAmbient));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Diffuse"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mDiffuse));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Specular"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mSpecular));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndGroup();
		ImGui::TreePop();
	}
	ImGui::PopID();
}

static void showSpotLightControls(SpotLight& light, int index)
{
	ImGui::PushID(index);
	if (ImGui::TreeNode("Spot Light"))
	{
		ImGui::BeginGroup();
		ImGui::SliderFloat3("Pos", glm::value_ptr(light.mPosition), -15.0f, 15.0f);
		ImGui::InputFloat3("Direction", glm::value_ptr(light.mDirection), 2);
		// TODO: clean up this messy shit!!
		static float innerAngle = acos(light.mInnerCutOff);
		static float outerAngle = acos(light.mOuterCutOff);
		ImGui::SliderFloat("Inner", &innerAngle, 0.0f, 180.0f);
		ImGui::SliderFloat("Outer", &outerAngle, 0.0f, 180.0f);
		outerAngle = utl::clamp(outerAngle, innerAngle, 180.0f);
		light.mInnerCutOff = cos(glm::radians(innerAngle));
		light.mOuterCutOff = cos(glm::radians(outerAngle));

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("Properties", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Ambient"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mAmbient));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Diffuse"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mDiffuse));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Specular"))
			{
				ImGui::ColorPicker3("", glm::value_ptr(light.mSpecular));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndGroup();
		ImGui::TreePop();
	}
	ImGui::PopID();
}
