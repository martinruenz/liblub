#include "Application.h"

class BufferTest : public Application {

public:
	BufferTest(){
		width = 1920;
		height = 1200;
		programName = "bufferTest";
  }

	void scene(){
		SceneGraph::Instance().light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));
		/*
		Material* material= new VertexColorMaterial();
		Material* material1= new NormalColorMat();
		*/
		Material* material= new ConvolutionMaterial();
		Mesh * plane = MeshFactory::Instance().load("monkey.blend");
		/*
		SceneGraph::Instance().addNode(plane,{-2,0,0}, material);
		SceneGraph::Instance().addNode(plane,{0,0,0}, material1);
		*/
		SceneGraph::Instance().addNode(new Node("",{0,0,-3},plane, material));
	}
};

int main(int argc, char *argv[])
{
	BufferTest().run();

}
