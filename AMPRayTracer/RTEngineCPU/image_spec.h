#pragma once
namespace rt_support
{
	/// <summary>
	/// 
	/// Supprots Image-specific information for the Ray Tracer command file:
	/// 
	///    1. Number of samples to be taken per pixel
	///    2. X/Y image resolution.
	/// 
	/// </summary>
	class image_spec
	{
	private:
		int m_samples_per_pixel;
		int m_x_resolution;
		int m_y_resolution;

	public:
		__declspec(dllexport) image_spec();
		__declspec(dllexport) image_spec(int x_resolution, int y_resolution, int samples_per_pixel);

		/// <summary>
		/// Access functions for the image spec. information
		/// </summary>
		int get_samples_per_pixel();
		__declspec(dllexport) int get_x_resolution();
		__declspec(dllexport) int get_y_resolution();
		float get_aspect_ratio();
	};
}