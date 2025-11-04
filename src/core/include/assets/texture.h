#pragma once


class Texture {
	private:
		unsigned int gl_id;
	public:
		Texture(std::string file);
		Texture() : gl_id(0) {};
		void Use();
};
