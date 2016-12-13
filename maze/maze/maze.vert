#version 130
uniform mat4 mvp;
uniform bool isBlock;
uniform bool isFloor;
in vec4 offset;
in vec2 texCoord;
in vec4 position;
in vec4 color;
out vec4 fragColor;
out vec2 fragTexCoord;
void main()
{
	if (isBlock) {
		gl_Position = mvp * (position + offset);
		fragTexCoord = texCoord;
		fragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}/*
	else if (isFloor) {
		gl_Position = mvp * (position + offset);
		fragTexCoord = texCoord;
		fragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}*/
	 else {
		gl_Position = mvp * position;
		fragColor = color;
		fragTexCoord = vec2(0.0f, 0.0f);
	}
}
