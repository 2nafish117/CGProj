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

	if (GLenum err = glewInit())
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

			//std::cout << camera.mPos.x << " " << camera.mPos.y << " " << camera.mPos.z << "\n";
			// std::cout << camera.mFront.x << " " << camera.mFront.y << " " << camera.mFront.z << "\n";
			{
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

				// key gives mouse access
				if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS)
				{
					glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					camera.mSensitivity = 0.0f;
				}
				if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_PRESS)
				{
					glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					camera.mSensitivity = SENSITIVITY;
				}

				double mouseXpos = 0, mouseYpos = 0 ;
				glfwGetCursorPos(mWindow, &mouseXpos, &mouseYpos);
				camera.handleMouseMoveInput(mouseXpos, mouseYpos, deltaTime);
				int leftButtonPressed = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT);
				int rightButtonPressed = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT);
				int middleButtonPressed = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT);
				camera.handleMouseButtonInput(leftButtonPressed, rightButtonPressed, middleButtonPressed, deltaTime);
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

# if 0
static void ShowDemoWindowLayout()
{
	if (!ImGui::CollapsingHeader("Layout"))
		return;

	if (ImGui::TreeNode("Child windows"))
	{
		static bool disable_mouse_wheel = false;
		static bool disable_menu = false;
		ImGui::Checkbox("Disable Mouse Wheel", &disable_mouse_wheel);
		ImGui::Checkbox("Disable Menu", &disable_menu);

		static int line = 50;
		bool goto_line = ImGui::Button("Goto");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		goto_line |= ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);

		// Child 1: no border, enable horizontal scrollbar
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0);
			ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 260), false, window_flags);
			for (int i = 0; i < 100; i++)
			{
				ImGui::Text("%04d: scrollable region", i);
				if (goto_line && line == i)
					ImGui::SetScrollHereY();
			}
			if (goto_line && line >= 100)
				ImGui::SetScrollHereY();
			ImGui::EndChild();
		}

		ImGui::SameLine();

		// Child 2: rounded border
		{
			ImGuiWindowFlags window_flags = (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0) | (disable_menu ? 0 : ImGuiWindowFlags_MenuBar);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("Child2", ImVec2(0, 260), true, window_flags);
			if (!disable_menu && ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Menu"))
				{
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Columns(2);
			for (int i = 0; i < 100; i++)
			{
				char buf[32];
				sprintf_s(buf, "%03d", i);
				ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
				ImGui::NextColumn();
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		ImGui::Separator();

		// Demonstrate a few extra things
		// - Changing ImGuiCol_ChildBg (which is transparent black in default styles)
		// - Using SetCursorPos() to position the child window (because the child window is an item from the POV of the parent window)
		//   You can also call SetNextWindowPos() to position the child window. The parent window will effectively layout from this position.
		// - Using ImGui::GetItemRectMin/Max() to query the "item" state (because the child window is an item from the POV of the parent window)
		//   See "Widgets" -> "Querying Status (Active/Focused/Hovered etc.)" section for more details about this.
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
			ImGui::BeginChild("blah", ImVec2(200, 100), true, ImGuiWindowFlags_None);
			for (int n = 0; n < 50; n++)
				ImGui::Text("Some test %d", n);
			ImGui::EndChild();
			ImVec2 child_rect_min = ImGui::GetItemRectMin();
			ImVec2 child_rect_max = ImGui::GetItemRectMax();
			ImGui::PopStyleColor();
			ImGui::Text("Rect of child window is: (%.0f,%.0f) (%.0f,%.0f)", child_rect_min.x, child_rect_min.y, child_rect_max.x, child_rect_max.y);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Widgets Width"))
	{
		// Use SetNextItemWidth() to set the width of a single upcoming item.
		// Use PushItemWidth()/PopItemWidth() to set the width of a group of items.
		static float f = 0.0f;
		ImGui::Text("SetNextItemWidth/PushItemWidth(100)");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::DragFloat("float##1", &f);

		ImGui::Text("SetNextItemWidth/PushItemWidth(GetWindowWidth() * 0.5f)");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5f);
		ImGui::DragFloat("float##2", &f);

		ImGui::Text("SetNextItemWidth/PushItemWidth(GetContentRegionAvail().x * 0.5f)");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::DragFloat("float##3", &f);

		ImGui::Text("SetNextItemWidth/PushItemWidth(-100)");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(-100);
		ImGui::DragFloat("float##4", &f);

		// Demonstrate using PushItemWidth to surround three items. Calling SetNextItemWidth() before each of them would have the same effect.
		ImGui::Text("SetNextItemWidth/PushItemWidth(-1)");
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		ImGui::DragFloat("##float5a", &f);
		ImGui::DragFloat("##float5b", &f);
		ImGui::DragFloat("##float5c", &f);
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Basic Horizontal Layout"))
	{
		ImGui::TextWrapped("(Use ImGui::SameLine() to keep adding items to the right of the preceding item)");

		// Text
		ImGui::Text("Two items: Hello"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

		// Adjust spacing
		ImGui::Text("More spacing: Hello"); ImGui::SameLine(0, 20);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

		// Button
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Normal buttons"); ImGui::SameLine();
		ImGui::Button("Banana"); ImGui::SameLine();
		ImGui::Button("Apple"); ImGui::SameLine();
		ImGui::Button("Corniflower");

		// Button
		ImGui::Text("Small buttons"); ImGui::SameLine();
		ImGui::SmallButton("Like this one"); ImGui::SameLine();
		ImGui::Text("can fit within a text block.");

		// Aligned to arbitrary position. Easy/cheap column.
		ImGui::Text("Aligned");
		ImGui::SameLine(150); ImGui::Text("x=150");
		ImGui::SameLine(300); ImGui::Text("x=300");
		ImGui::Text("Aligned");
		ImGui::SameLine(150); ImGui::SmallButton("x=150");
		ImGui::SameLine(300); ImGui::SmallButton("x=300");

		// Checkbox
		static bool c1 = false, c2 = false, c3 = false, c4 = false;
		ImGui::Checkbox("My", &c1); ImGui::SameLine();
		ImGui::Checkbox("Tailor", &c2); ImGui::SameLine();
		ImGui::Checkbox("Is", &c3); ImGui::SameLine();
		ImGui::Checkbox("Rich", &c4);

		// Various
		static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
		ImGui::PushItemWidth(80);
		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
		static int item = -1;
		ImGui::Combo("Combo", &item, items, IM_ARRAYSIZE(items)); ImGui::SameLine();
		ImGui::SliderFloat("X", &f0, 0.0f, 5.0f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &f1, 0.0f, 5.0f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &f2, 0.0f, 5.0f);
		ImGui::PopItemWidth();

		ImGui::PushItemWidth(80);
		ImGui::Text("Lists:");
		static int selection[4] = { 0, 1, 2, 3 };
		for (int i = 0; i < 4; i++)
		{
			if (i > 0) ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
			ImGui::PopID();
			//if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
		}
		ImGui::PopItemWidth();

		// Dummy
		ImVec2 button_sz(40, 40);
		ImGui::Button("A", button_sz); ImGui::SameLine();
		ImGui::Dummy(button_sz); ImGui::SameLine();
		ImGui::Button("B", button_sz);

		// Manually wrapping (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
		ImGui::Text("Manually wrapping:");
		ImGuiStyle& style = ImGui::GetStyle();
		int buttons_count = 20;
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		for (int n = 0; n < buttons_count; n++)
		{
			ImGui::PushID(n);
			ImGui::Button("Box", button_sz);
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
			if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
			ImGui::PopID();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Tabs"))
	{
		if (ImGui::TreeNode("Basic"))
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Avocado"))
				{
					ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Broccoli"))
				{
					ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Cucumber"))
				{
					ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Advanced & Close Button"))
		{
			// Expose a couple of the available flags. In most cases you may just call BeginTabBar() with no flags (0).
			static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
			ImGui::CheckboxFlags("ImGuiTabBarFlags_Reorderable", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_Reorderable);
			ImGui::CheckboxFlags("ImGuiTabBarFlags_AutoSelectNewTabs", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_AutoSelectNewTabs);
			ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
			ImGui::CheckboxFlags("ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
			if ((tab_bar_flags & ImGuiTabBarFlags_FittingPolicyMask_) == 0)
				tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyDefault_;
			if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
				tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
			if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
				tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

			// Tab Bar
			const char* names[4] = { "Artichoke", "Beetroot", "Celery", "Daikon" };
			static bool opened[4] = { true, true, true, true }; // Persistent user state
			for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
			{
				if (n > 0) { ImGui::SameLine(); }
				ImGui::Checkbox(names[n], &opened[n]);
			}

			// Passing a bool* to BeginTabItem() is similar to passing one to Begin(): the underlying bool will be set to false when the tab is closed.
			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
					if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n]))
					{
						ImGui::Text("This is the %s tab!", names[n]);
						if (n & 1)
							ImGui::Text("I am an odd tab.");
						ImGui::EndTabItem();
					}
				ImGui::EndTabBar();
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Groups"))
	{
		ImGui::BeginGroup();
		{
			ImGui::BeginGroup();
			ImGui::Button("AAA");
			ImGui::SameLine();
			ImGui::Button("BBB");
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Button("CCC");
			ImGui::Button("DDD");
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::Button("EEE");
			ImGui::EndGroup();
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("First group hovered");
		}
		// Capture the group size and create widgets using the same size
		ImVec2 size = ImGui::GetItemRectSize();
		const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
		ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

		ImGui::Button("ACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
		ImGui::SameLine();
		ImGui::Button("REACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::Button("LEVERAGE\nBUZZWORD", size);
		ImGui::SameLine();

		if (ImGui::ListBoxHeader("List", size))
		{
			ImGui::Selectable("Selected", true);
			ImGui::Selectable("Not Selected", false);
			ImGui::ListBoxFooter();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Text Baseline Alignment"))
	{

		ImGui::Text("One\nTwo\nThree"); ImGui::SameLine();
		ImGui::Text("Hello\nWorld"); ImGui::SameLine();
		ImGui::Text("Banana");

		ImGui::Text("Banana"); ImGui::SameLine();
		ImGui::Text("Hello\nWorld"); ImGui::SameLine();
		ImGui::Text("One\nTwo\nThree");

		ImGui::Button("HOP##1"); ImGui::SameLine();
		ImGui::Text("Banana"); ImGui::SameLine();
		ImGui::Text("Hello\nWorld"); ImGui::SameLine();
		ImGui::Text("Banana");

		ImGui::Button("HOP##2"); ImGui::SameLine();
		ImGui::Text("Hello\nWorld"); ImGui::SameLine();
		ImGui::Text("Banana");

		ImGui::Button("TEST##1"); ImGui::SameLine();
		ImGui::Text("TEST"); ImGui::SameLine();
		ImGui::SmallButton("TEST##2");

		ImGui::AlignTextToFramePadding(); // If your line starts with text, call this to align it to upcoming widgets.
		ImGui::Text("Text aligned to Widget"); ImGui::SameLine();
		ImGui::Button("Widget##1"); ImGui::SameLine();
		ImGui::Text("Widget"); ImGui::SameLine();
		ImGui::SmallButton("Widget##2"); ImGui::SameLine();
		ImGui::Button("Widget##3");

		// Tree
		const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGui::Button("Button##1");
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::TreeNode("Node##1")) { for (int i = 0; i < 6; i++) ImGui::BulletText("Item %d..", i); ImGui::TreePop(); }    // Dummy tree data

		ImGui::AlignTextToFramePadding();         // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget. Otherwise you can use SmallButton (smaller fit).
		bool node_open = ImGui::TreeNode("Node##2");  // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add child content.
		ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##2");
		if (node_open) { for (int i = 0; i < 6; i++) ImGui::BulletText("Item %d..", i); ImGui::TreePop(); }   // Dummy tree data

		// Bullet
		ImGui::Button("Button##3");
		ImGui::SameLine(0.0f, spacing);
		ImGui::BulletText("Bullet text");

		ImGui::AlignTextToFramePadding();
		ImGui::BulletText("Node");
		ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##4");

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Scrolling"))
	{
		// Vertical scroll functions
		static int track_item = 50;
		static bool enable_track = true;
		static bool enable_extra_decorations = false;
		static float scroll_to_off_px = 0.0f;
		static float scroll_to_pos_px = 200.0f;

		ImGui::Checkbox("Decoration", &enable_extra_decorations);
		ImGui::SameLine();

		ImGui::Checkbox("Track", &enable_track);
		ImGui::PushItemWidth(100);
		ImGui::SameLine(140); enable_track |= ImGui::DragInt("##item", &track_item, 0.25f, 0, 99, "Item = %d");

		bool scroll_to_off = ImGui::Button("Scroll Offset");
		ImGui::SameLine(140); scroll_to_off |= ImGui::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, 9999, "+%.0f px");

		bool scroll_to_pos = ImGui::Button("Scroll To Pos");
		ImGui::SameLine(140); scroll_to_pos |= ImGui::DragFloat("##pos", &scroll_to_pos_px, 1.00f, -10, 9999, "X/Y = %.0f px");
		ImGui::PopItemWidth();

		if (scroll_to_off || scroll_to_pos)
			enable_track = false;

		ImGuiStyle& style = ImGui::GetStyle();
		float child_w = (ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
		if (child_w < 1.0f)
			child_w = 1.0f;
		ImGui::PushID("##VerticalScrolling");
		for (int i = 0; i < 5; i++)
		{
			if (i > 0) ImGui::SameLine();
			ImGui::BeginGroup();
			const char* names[] = { "Top", "25%", "Center", "75%", "Bottom" };
			ImGui::TextUnformatted(names[i]);

			ImGuiWindowFlags child_flags = enable_extra_decorations ? ImGuiWindowFlags_MenuBar : 0;
			ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(child_w, 200.0f), true, child_flags);
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted("abc");
				ImGui::EndMenuBar();
			}
			if (scroll_to_off)
				ImGui::SetScrollY(scroll_to_off_px);
			if (scroll_to_pos)
				ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, i * 0.25f);
			for (int item = 0; item < 100; item++)
			{
				if (enable_track && item == track_item)
				{
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
					ImGui::SetScrollHereY(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
				}
				else
				{
					ImGui::Text("Item %d", item);
				}
			}
			float scroll_y = ImGui::GetScrollY();
			float scroll_max_y = ImGui::GetScrollMaxY();
			ImGui::EndChild();
			ImGui::Text("%.0f/%.0f", scroll_y, scroll_max_y);
			ImGui::EndGroup();
		}
		ImGui::PopID();

		// Horizontal scroll functions
		ImGui::Spacing();

		ImGui::PushID("##HorizontalScrolling");
		for (int i = 0; i < 5; i++)
		{
			float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
			ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
			ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(-100, child_height), true, child_flags);
			if (scroll_to_off)
				ImGui::SetScrollX(scroll_to_off_px);
			if (scroll_to_pos)
				ImGui::SetScrollFromPosX(ImGui::GetCursorStartPos().x + scroll_to_pos_px, i * 0.25f);
			for (int item = 0; item < 100; item++)
			{
				if (enable_track && item == track_item)
				{
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
					ImGui::SetScrollHereX(i * 0.25f); // 0.0f:left, 0.5f:center, 1.0f:right
				}
				else
				{
					ImGui::Text("Item %d", item);
				}
				ImGui::SameLine();
			}
			float scroll_x = ImGui::GetScrollX();
			float scroll_max_x = ImGui::GetScrollMaxX();
			ImGui::EndChild();
			ImGui::SameLine();
			const char* names[] = { "Left", "25%", "Center", "75%", "Right" };
			ImGui::Text("%s\n%.0f/%.0f", names[i], scroll_x, scroll_max_x);
			ImGui::Spacing();
		}
		ImGui::PopID();

		// Miscellaneous Horizontal Scrolling Demo

		static int lines = 7;
		ImGui::SliderInt("Lines", &lines, 1, 15);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
		ImGui::BeginChild("scrolling", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30), true, ImGuiWindowFlags_HorizontalScrollbar);
		for (int line = 0; line < lines; line++)
		{
			// Display random stuff (for the sake of this trivial demo we are using basic Button+SameLine. If you want to create your own time line for a real application you may be better off
			// manipulating the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets yourself. You may also want to use the lower-level ImDrawList API)
			int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
			for (int n = 0; n < num_buttons; n++)
			{
				if (n > 0) ImGui::SameLine();
				ImGui::PushID(n + line * 1000);
				char num_buf[16];
				sprintf_s(num_buf, "%d", n);
				const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : num_buf;
				float hue = n * 0.05f;
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
				ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
				ImGui::PopStyleColor(3);
				ImGui::PopID();
			}
		}
		float scroll_x = ImGui::GetScrollX();
		float scroll_max_x = ImGui::GetScrollMaxX();
		ImGui::EndChild();
		ImGui::PopStyleVar(2);
		float scroll_x_delta = 0.0f;
		ImGui::SmallButton("<<"); if (ImGui::IsItemActive()) { scroll_x_delta = -ImGui::GetIO().DeltaTime * 1000.0f; } ImGui::SameLine();
		ImGui::Text("Scroll from code"); ImGui::SameLine();
		ImGui::SmallButton(">>"); if (ImGui::IsItemActive()) { scroll_x_delta = +ImGui::GetIO().DeltaTime * 1000.0f; } ImGui::SameLine();
		ImGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
		if (scroll_x_delta != 0.0f)
		{
			ImGui::BeginChild("scrolling"); // Demonstrate a trick: you can use Begin to set yourself in the context of another window (here we are already out of your child window)
			ImGui::SetScrollX(ImGui::GetScrollX() + scroll_x_delta);
			ImGui::EndChild();
		}
		ImGui::Spacing();

		static bool show_horizontal_contents_size_demo_window = false;
		ImGui::Checkbox("Show Horizontal contents size demo window", &show_horizontal_contents_size_demo_window);

		if (show_horizontal_contents_size_demo_window)
		{
			static bool show_h_scrollbar = true;
			static bool show_button = true;
			static bool show_tree_nodes = true;
			static bool show_text_wrapped = false;
			static bool show_columns = true;
			static bool show_tab_bar = true;
			static bool show_child = false;
			static bool explicit_content_size = false;
			static float contents_size_x = 300.0f;
			if (explicit_content_size)
				ImGui::SetNextWindowContentSize(ImVec2(contents_size_x, 0.0f));
			ImGui::Begin("Horizontal contents size demo window", &show_horizontal_contents_size_demo_window, show_h_scrollbar ? ImGuiWindowFlags_HorizontalScrollbar : 0);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
			ImGui::Checkbox("H-scrollbar", &show_h_scrollbar);
			ImGui::Checkbox("Button", &show_button);            // Will grow contents size (unless explicitly overwritten)
			ImGui::Checkbox("Tree nodes", &show_tree_nodes);    // Will grow contents size and display highlight over full width
			ImGui::Checkbox("Text wrapped", &show_text_wrapped);// Will grow and use contents size
			ImGui::Checkbox("Columns", &show_columns);          // Will use contents size
			ImGui::Checkbox("Tab bar", &show_tab_bar);          // Will use contents size
			ImGui::Checkbox("Child", &show_child);              // Will grow and use contents size
			ImGui::Checkbox("Explicit content size", &explicit_content_size);
			ImGui::Text("Scroll %.1f/%.1f %.1f/%.1f", ImGui::GetScrollX(), ImGui::GetScrollMaxX(), ImGui::GetScrollY(), ImGui::GetScrollMaxY());
			if (explicit_content_size)
			{
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				ImGui::DragFloat("##csx", &contents_size_x);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 10, p.y + 10), IM_COL32_WHITE);
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + contents_size_x - 10, p.y), ImVec2(p.x + contents_size_x, p.y + 10), IM_COL32_WHITE);
				ImGui::Dummy(ImVec2(0, 10));
			}
			ImGui::PopStyleVar(2);
			ImGui::Separator();
			if (show_button)
			{
				ImGui::Button("this is a 300-wide button", ImVec2(300, 0));
			}
			if (show_tree_nodes)
			{
				bool open = true;
				if (ImGui::TreeNode("this is a tree node"))
				{
					if (ImGui::TreeNode("another one of those tree node..."))
					{
						ImGui::Text("Some tree contents");
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				ImGui::CollapsingHeader("CollapsingHeader", &open);
			}
			if (show_text_wrapped)
			{
				ImGui::TextWrapped("This text should automatically wrap on the edge of the work rectangle.");
			}
			if (show_columns)
			{
				ImGui::Columns(4);
				for (int n = 0; n < 4; n++)
				{
					ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
					ImGui::NextColumn();
				}
				ImGui::Columns(1);
			}
			if (show_tab_bar && ImGui::BeginTabBar("Hello"))
			{
				if (ImGui::BeginTabItem("OneOneOne")) { ImGui::EndTabItem(); }
				if (ImGui::BeginTabItem("TwoTwoTwo")) { ImGui::EndTabItem(); }
				if (ImGui::BeginTabItem("ThreeThreeThree")) { ImGui::EndTabItem(); }
				if (ImGui::BeginTabItem("FourFourFour")) { ImGui::EndTabItem(); }
				ImGui::EndTabBar();
			}
			if (show_child)
			{
				ImGui::BeginChild("child", ImVec2(0, 0), true);
				ImGui::EndChild();
			}
			ImGui::End();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Clipping"))
	{
		static ImVec2 size(100, 100), offset(50, 20);
		ImGui::TextWrapped("On a per-widget basis we are occasionally clipping text CPU-side if it won't fit in its frame. Otherwise we are doing coarser clipping + passing a scissor rectangle to the renderer. The system is designed to try minimizing both execution and CPU/GPU rendering cost.");
		ImGui::DragFloat2("size", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
		ImGui::TextWrapped("(Click and drag)");
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec4 clip_rect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		ImGui::InvisibleButton("##dummy", size);
		if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) { offset.x += ImGui::GetIO().MouseDelta.x; offset.y += ImGui::GetIO().MouseDelta.y; }
		ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(90, 90, 120, 255));
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(pos.x + offset.x, pos.y + offset.y), IM_COL32(255, 255, 255, 255), "Line 1 hello\nLine 2 clip me!", NULL, 0.0f, &clip_rect);
		ImGui::TreePop();
	}
}
#endif