#include "SceneGraph.h"

///////////////////////////////// SCENE NODE
SceneNode::SceneNode() {}

SceneNode::~SceneNode() {
	for (SceneNode* child : children) {
		delete child;
	}
}

SceneNode::SceneNode(Mesh* mesh, const Mat4& model, SceneNode* parent, ShaderProgram* shaderProgram)
	: mesh(mesh), model(model), parent(parent), shaderProgram(shaderProgram) {}

SceneNode* SceneNode::createChild() {
	SceneNode* child = new SceneNode();
	child->parent = this;
	child->shaderProgram = shaderProgram;
	child->mesh = mesh;
	children.push_back(child);
	return child;
}

SceneNode* SceneNode::createChild(Mesh* mesh, const Mat4& model) {
	// This code might be a little bit confusing because the user
	// might not realize that the child is inheriting the shaderProgram and beforeDraw
	SceneNode* child = new SceneNode(mesh, model, this, shaderProgram);
	if (this->beforeDraw)
		child->beforeDraw = this->beforeDraw;
	if (this->afterDraw)
		child->afterDraw = this->afterDraw;
	children.push_back(child);
	return child;
}

SceneNode* SceneNode::createChild(Mesh* mesh, const Mat4& model, ShaderProgram* shaderProgram) {
	SceneNode* child = new SceneNode(mesh, model, this, shaderProgram);
	if (this->beforeDraw)
		child->beforeDraw = this->beforeDraw;
	if (this->afterDraw)
		child->afterDraw = this->afterDraw;
	children.push_back(child);
	return child;
}

void SceneNode::setModel(const Mat4& model) {
	this->model = model;
	modelChanged = true;
}

void SceneNode::setShaderProgram(ShaderProgram* shaderProgram) {
	this->shaderProgram = shaderProgram;
}

void SceneNode::addTexture(ITexture* texture) {
	this->textures.push_back(texture);
}

void SceneNode::setBeforeDrawFunction(const std::function<void(ShaderProgram*)>& beforeDraw){
	this->beforeDraw = beforeDraw;
}

void SceneNode::setAfterDrawFunction(const std::function<void()>& afterDraw) {
	this->afterDraw = afterDraw;
}

Mat4 SceneNode::getModel() const{
	return model;
}
Mesh* SceneNode::getMesh() const{
	return mesh;
}
ShaderProgram* SceneNode::getShaderProgram() const{
	return shaderProgram;
}
std::vector<SceneNode*> SceneNode::getChildren() const {
	return children;
}

std::vector<ITexture*> SceneNode::getTextures() const {
	return textures;
}

void SceneNode::init() {
	if (mesh != nullptr) {
		mesh->init();
		modelUniformLocation = shaderProgram->getUniformLocation("model");
		normalUniformLocation = shaderProgram->getUniformLocation("normal");
	}
	for (SceneNode* child : children) {
		child->init();
	}
}

Mat4 SceneNode::retriveModelRecursively() {
	if (parent == nullptr)
		return model;
	return parent->retriveModelRecursively() * model;
}

void SceneNode::draw() {

	for (SceneNode* child : children) {
		child->draw();
	}
	
	if (mesh != nullptr) {

		shaderProgram->use();
		mesh->bind();

		for (int i = 0; i < textures.size(); ++i) {
			textures[i]->bind(GL_TEXTURE0 + i);
		}

		if(beforeDraw)
			beforeDraw(shaderProgram);

		Mat4 model = retriveModelRecursively();
		shaderProgram->setUniform(modelUniformLocation, model);
		Mat3 inverse;
		bool canInverse = model.toMat3().inverse(inverse);
		shaderProgram->setUniform(normalUniformLocation, canInverse ? inverse.transpose() : Mat3::ZERO);
		modelChanged = false;

		mesh->draw();

		if (afterDraw)
			afterDraw();

		for (int i = 0; i < textures.size(); ++i) {
			textures[i]->unBind(GL_TEXTURE0 + i);
		}

		mesh->unBind();
		shaderProgram->stopUsing();
	}

	
}

////////////////////////////////// SCENE GRAPH

SceneGraph::SceneGraph(Camera* camera) : root(new SceneNode()), camera(camera) {}

SceneGraph::~SceneGraph() {
	delete root;
}

void SceneGraph::init() {
	root->init();
}

void SceneGraph::draw(const float elapsedTime) {
	camera->update(elapsedTime);
	root->draw();
}

SceneNode* SceneGraph::createChild() {
	return root->createChild();
}

SceneNode* SceneGraph::getRoot() const{
	return root;
}

Camera* SceneGraph::getCamera() const{
	return camera;
}