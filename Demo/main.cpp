#include <Dragonfly/editor.h>		 //inlcludes most features
#include <Dragonfly/detail/buffer.h> //will be replaced
#include <Dragonfly/detail/vao.h>	 //will be replaced
#include <Dragonfly/detail/FileIO/FileIO.h>

#include <Dragonfly/detail/Buffer/UniformBuffer.h>

#include <string>

int main(int argc, char* args[])
{
	df::Sample sam("Dragonfly Demo", 720, 480, df::Sample::FLAGS::DEFAULT);
	// df::Sample simplifies OpenGL, SDL, ImGui, RenderDoc in the render loop, and handles user input via callback member functions in priority queues
	df::Camera cam;								// Implements a camera event class with handles
	sam.AddHandlerClass(cam, 5);				// class callbacks will be called to change its state
	sam.AddHandlerClass<df::ImGuiHandler>(10);	// static handle functions only

	eltecg::ogl::ArrayBuffer MyVBO;	MyVBO.constructMutable(std::vector<glm::vec2>{ {-1, -1}, { 1, -1 }, { 0, 1 }}, GL_STATIC_DRAW);
	eltecg::ogl::VertexArray MyVAO;	MyVAO.addVBO<glm::vec2>(MyVBO);		//these two classes will be removed from Dragonfly as soon as we have the replacement ready

	df::VaoArrays demoVao((GLuint)MyVAO, GL_TRIANGLE_STRIP, 3, 0u); // temporary solution that wraps an ID

	df::TextureCube<> testCubemap("Assets/xpos.png", "Assets/xneg.png", "Assets/ypos.png", "Assets/yneg.png", "Assets/zpos.png", "Assets/zneg.png");
	df::Texture2D<> testTex = testCubemap[df::X_POS]; // 2D view of a cubemap face

	df::ShaderProgramEditorVF program = "MyShaderProgram";
	program << "Shaders/vert.vert"_vert << "Shaders/frag.frag"_frag << df::LinkProgram;
	std::cout << program.GetErrors() << std::endl;
	df::ShaderProgramEditorVF postprocess = "Postprocess shader program";
	postprocess << "Shaders/postprocess.vert"_vert << "Shaders/postprocess.frag"_frag << df::LinkProgram;
	std::cout << postprocess.GetErrors() << std::endl;

	int w = df::Backbuffer.getWidth(), h = df::Backbuffer.getHeight();
	auto frameBuff = df::Renderbuffer<df::depth24>(w, h) + df::Texture2D<>(w, h, 1);

	//sam.AddLogger([&](const df::LogEntry_Vec& logger, uint64_t frame_number_) {
	//	std::cout << "--- Frame: " << frame_number_ << std::endl;
	//	for (const auto& asd : logger)
	//		std::cout << asd.message << std::endl;
	//	});
	sam.AddResize([&](int w, int h) {frameBuff = frameBuff.MakeResized(w, h); });

	GL_CHECK; //extra opengl error checking in GPU Debug build configuration

	//df::Logger.AddEntry(df::detail::Logger::Entry(
	//	df::detail::Logger::Entry::SEVERITY::INFO,
	//	df::detail::Logger::Entry::Location{ "path",12 },
	//	"expr", "Testing if Logger works.",
	//	df::detail::Logger::Entry::TYPE::USER
	//));





	//// Shaders
	//std::string asd;


	///*
	//=========================================================
	//                                    Shader source
	//=========================================================
	//*/

	//std::string code = my_code_generation();


	//auto /*df::detail::ShaderSource*/ source = "#version 430\n"_code + "uniform vec3 color;\n"_code + "shaders/vs/file.h"_file;
	//auto /*df::detail::VertexShaderSource*/ vs_sourcelist1 = "file1"_vs + "file2"_vs;
	//vs_sourcelist1.SetPath("shaders/vs/");
	//auto /*df::detail::VertexShaderSource*/ vs_sourcelist2 = "#define DEBUG"_code + "file1"_vs + Code(code);
	//// Cannot add different typed shader sources
	//// Only list of sources, files are yet to be read/loaded 	

	//auto shade_source = "shade.glsl"_forceLoad; // { {FILE, "file path", "source"}	} <- loaded file / file path / string
	//auto half_source = "#define PI 3"_code + "trace.glsl"_fs + shade_source;

	//auto full_source = "#define RELEASE"_code + half_source;
	//auto full_source = "#define DEBUG"_code + half_source;

	///*
	//=========================================================
	//                                    (Pre)compiled Shader
	//=========================================================
	//*/


	//auto /*VertexShader*/ compiledVS = MakeShader("#define RELEASE"_code + vs_sourcelist1);
	//auto /*VertexShader*/ compiledVS = VertexShader("#define RELEASE"_code + vs_sourcelist1); //equivalent
	//// cannot add compiled shaders togather


	///*
	//=========================================================
	//                                        Program
	//=========================================================
	//*/


	//auto myProg = MakeProgram("vs.vert"_vs, "fs.frag"_fs + "fs.frag"_fs);
	//auto myProg = MakeProgram(compiledVS, "fs.frag"_fs + "fs.frag"_fs);
	//// MakeProgram should have a counter for each shader. Valid programs:
	//        // (a single) compute shader or
	//        // vs + [tc + te] + [geom] + fs
	//        // in any order seperated by commas

	//// Constructors
	//auto myVs = MakeVertexShader("vert.glsl"_file); // Read from file
	//auto myFs = MakeFragmentShaderEditor("vert.hlsl"_file, "long code"); // For code generation


	//auto vs1 = "file1"_vsfile;
	//auto vs2 = "file2"_vsfile;

	//auto fs = MakeShader("file"_fsfile);

	////fs.Compile();

	//auto fixvs = vs1 + vs2;

	//vector programs;
	//for (auto asd : asd) {
	//    auto tempvs = vs(asd);
	//    programs.pushback(program(vs1 + vs2 + tempvs + fs))
	//        programs.pushback(program(fixvs + tempvs + fs))
	//}



	//auto vs = "file.vs"_vsfile;

	///*
	//Program
	//*/

	//// Constructors
	//auto myProg1 = myVs + MakeFragmentShader("vert.hlsl"_file, "another generated code"); //none of the shaders are editors
	//auto myProg2 = myVs + myFs;   	// one is an editor --> this promotes myProg2 to an Editor as well so you can do the following from UI
	//        // Program Editor UI
	//        // - Compile, link, error list
	//        // - Uniform view: Buffers, Textures, and Subroutines


	//// Rendering the UI:
	//// Old way: myFs.RenderUI(); //renders the generated UI
	//// Each program editor is added to a global list
	//// The EditorsEventHandler renders each ProgramEditor within the global list when added to Sample sam.

	//myProg["myUbo"]["view"] = glm::mat4(1);

	//myProg[]
	//df::Buffer<glm::vec3, float> buff;

	//auto vao = buff + buff;

	//myProg["myUbo"] = buff;

	//myProg << "myUbo" << buff;


	///**/


	program << "texImg" << testTex;
	GLint progID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &progID);
	
	auto [ubo_layouts, unbuffered] = programParseUniformProperties(progID);

	auto asd = ubo_layouts.find("MyBlock");
	
	df::UniformBuffer ubo{asd->second};
	ubo.Bind(3); //binding point
	glUniformBlockBinding(progID, 0, 3);
	
	ubo["v"] = glm::vec3(1,1,0);
	ubo["f1"] = 0.0f;
	ubo.UploadBuffer();
	
	sam.Run([&](float deltaTime) //delta time in ms
		{
			bool uboChanged = false;
			uboChanged |= ImGui::ColorEdit3("color", ubo["v"].Get<float>());
			uboChanged |= ImGui::SliderFloat("f1mix",ubo["f1"].Get<float>(),0,1);
			if(uboChanged)
				ubo.UploadBuffer();
		
			cam.Update();

			frameBuff << df::Clear() << program << "texImg" << testTex;
			program << demoVao;	//Rendering: Ensures that both the vao and program is attached

			df::Backbuffer << df::Clear() << postprocess << "texFrame" << frameBuff.get<glm::u8vec3>();
			postprocess << df::NoVao(GL_TRIANGLES, 3); // Rendering a pixel shader

			GL_CHECK;
			program.Render(); postprocess.Render(); //only the UI!!
		}
	);
	return 0;
}
