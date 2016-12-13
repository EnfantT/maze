#version 130

#define HASH(in1, in2) (int(fract(sin(dot(vec2(in1+2*in2, in1+in2),vec2(12.9898,78.233))) * 43758.5453) > 0.5))

vec4 TileTex(sampler2D texSampler, vec2 uv)
{
    //uv.y = 1.0-uv.y;
	    
	vec2 t = floor(uv);
    
	int tileX = int(t.x);
	int tileY = int(t.y);

    vec2 tile = vec2(0.0, 0.0);    
   
    // use the hashing algorithm

    // BRTL - Bottom, Rright, Top, Left
    int B = HASH(tileY-1, tileX);
    int R = HASH(tileX+1, tileY);
    int T = HASH(tileY, tileX);
    int L = HASH(tileX, tileY);	    

    const vec2 tiles[16] = 
    {
        vec2(0.0, 3.0), // 0, 0, 0, 0     // 0
        vec2(3.0, 3.0), // 0, 0, 0, 1     // 1
        vec2(0.0, 2.0), // 0, 0, 1, 0     // 2
        vec2(3.0, 2.0), // 0, 0, 1, 1     // 3
        vec2(1.0, 3.0), // 0, 1, 0, 0     // 4
        vec2(2.0, 3.0), // 0, 1, 0, 1     // 5
        vec2(1.0, 2.0), // 0, 1, 1, 0     // 6
        vec2(2.0, 2.0), // 0, 1, 1, 1     // 7
        vec2(0.0, 0.0), // 1, 0, 0, 0     // 8
        vec2(3.0, 0.0), // 1, 0, 0, 1     // 9 
        vec2(0.0, 1.0), // 1, 0, 1, 0     // 10
        vec2(3.0, 1.0), // 1, 0, 1, 1     // 11
        vec2(1.0, 0.0), // 1, 1, 0, 0     // 12
        vec2(2.0, 0.0), // 1, 1, 0, 1     // 13
        vec2(1.0, 1.0), // 1, 1, 1, 0     // 14
        vec2(2.0, 1.0), // 1, 1, 1, 1     // 15
    };
        
	tile = tiles[L + 2*T + 4*R + 8*B];
    	
	vec2 finalCoords;
	finalCoords.x = (tile.x/4.0) + fract(uv.x)/4.0;
	finalCoords.y = ((3.0-tile.y)/4.0) + fract(uv.y)/4.0;
	  
	finalCoords.y = 1.0-finalCoords.y;
	return textureGrad(texSampler, finalCoords, dFdx(uv/4.0), dFdy(uv/4.0));
	 
}
	
in vec4 fragColor;
in vec2 fragTexCoord;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform bool isBlock;

void main()
{
	
	if (isBlock) {
		gl_FragColor = TileTex(tex, fragTexCoord * vec2(4.0f, 4.0f));		
	} else {
		gl_FragColor = fragColor;
	}
}