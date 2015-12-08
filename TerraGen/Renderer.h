#pragma once


class Renderer
{
public:

	static bool ShadowsEnabled;
	static glm::vec3 BackgroundColor;

	/// <summary>
	/// Creates a new Renderer
	/// </summary>
	/// <param name="width">The width of the window</param>
	/// <param name="height">The height of the window</param>
	Renderer(int width, int height);

	/// <summary>
	/// Destroys the Renderer
	/// </summary>
	~Renderer();

	/// <summary>
	/// Resizes the renderer to the given dimensions
	/// </summary>
	/// <param name="height"></param>
	void resize(int width, int height);

	/// <summary>
	/// Returns the size of the renderer
	/// </summary>
	void getSize(int& width, int& height);

	/// <summary>
	/// Updates the Renderer to the current state.
	/// </summary>
	/// <param name="deltaT"></param>
	void update(float deltaT);

	/// <summary>
	/// Draws the given scene to the screen
	/// </summary>
	/// <param name="scene"></param>
	void render(SceneGraph* scene);

private:

	int width;

	int height;

	/// <summary>
	/// Renders shadows into the framebuffer
	/// </summary>
	/// <param name="light">The light that casts shadows</param>
	void renderShadowMap(DirectionalLight* light);

	/// <summary>
	/// A Framebuffer which stores the shadow map
	/// </summary>
    //Framebuffer shadowMapFBO;

	/// <summary>
	/// Screen Space quad for deferred rendering and post process
	/// </summary>
    //Quad* quad;
};

