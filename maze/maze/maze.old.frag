#version 130
in vec4 fragColor;
in vec2 fragTexCoord;
uniform sampler2D tex;
uniform bool isBlock;
void main()
{
	if (isBlock) {
		gl_FragColor = texture2D(tex, fragTexCoord);
	} else {
		gl_FragColor = fragColor;
	}
}
