#version 130
#define GLSL
#define FLIP_V

// just a sample that guarantees using the highest mip level
/*
#ifndef GRID_SAMPLE
        #define GRID_SAMPLE(uv) texture2D(wangSampler, uv, -999.0)
#endif
*/

#define HASH(in1, in2) (int(fract(sin(dot(vec2(in1+2*in2, in1+in2),vec2(12.9898,78.233))) * 43758.5453) > 0.5))

vec4 wangSample(sampler2D texSampler, vec2 uv)
{
    //uv.y = 1.0-uv.y;

    
	vec2 t = floor(uv);
    
	int tileX = int(t.x);
	int tileY = int(t.y);

    vec2 tile = vec2(0.0, 0.0);    
   
    // use the hashing algorithm

    // senw - South, East, North, West
    int s = HASH(tileY-1, tileX);
    int e = HASH(tileX+1, tileY);
    int n = HASH(tileY, tileX);
    int w = HASH(tileX, tileY);

    // turn senw values into a tile specifier (x,y coord) of the tile to use (from the tileset)        
    // in webgl we can't do lookup tables, so we use a big 'ol nasty if-else chain

    #ifndef NO_LOOKUP_TABLES
    {
        /* static */ const vec2 tiles[16] = 
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
        
		tile = tiles[w + 2*n + 4*e + 8*s];
				
    }    
    #else // ndef NO_LOOKUP_TABLES
    {
        if(s == 0)
        {
            if(e == 0)
            {
                if(n == 0)
                {
                    if(w == 0)
                        tile = vec2(0.0, 3.0); // 0, 0, 0, 0
                    else // w == 1.0
                        tile = vec2(3.0, 3.0); // 0, 0, 0, 1
                }
                else // n == 1.0
                {
                    if(w == 0)
                        tile = vec2(0.0, 2.0); // 0, 0, 1, 0
                    else // w == 1.0
                        tile = vec2(3.0, 2.0); // 0, 0, 1, 1
                }   
            }
            else // e == 1.0
            {
                if(n == 0)
                {
                    if(w == 0)
                        tile = vec2(1.0, 3.0); // 0, 1, 0, 0
                    else // w == 1.0
                        tile = vec2(2.0, 3.0); // 0, 1, 0, 1
                }
                else // n == 1.0
                {
                    if(w == 0)
                        tile = vec2(1.0, 2.0); // 0, 1, 1, 0
                    else // w == 1.0
                        tile = vec2(2.0, 2.0); // 0, 1, 1, 1
                }   
            }   
        }   
        else // s == 1.0
        {
            if(e == 0)
            {
                if(n == 0)
                {
                    if(w == 0)
                        tile = vec2(0.0, 0.0); // 1, 0, 0, 0
                    else // w == 1.0
                        tile = vec2(3.0, 0.0); // 1, 0, 0, 1
                }
                else // n == 1.0
                {
                    if(w == 0)
                        tile = vec2(0.0, 1.0); // 1, 0, 1, 0
                    else // w == 1.0
                        tile = vec2(3.0, 1.0); // 1, 0, 1, 1
                }
            }
            else // e == 1.0
            {
                if(n == 0)
                {
                    if(w == 0)
                        tile = vec2(1.0, 0.0); // 1, 1, 0, 0
                    else // w == 1.0
                        tile = vec2(2.0, 0.0); // 1, 1, 0, 1
                }
                else // n == 1.0
                {
                    if(w == 0)
                        tile = vec2(1.0, 1.0); // 1, 1, 1, 0
                    else // w == 1.0
                        tile = vec2(2.0, 1.0); // 1, 1, 1, 1
                }
            }
        }
    }
    #endif // ndef NO_LOOKUP_TABLES
    
	// calculate the final texture coordinates from the computed tile value
	vec2 finalCoords;
	finalCoords.x = (tile.x/4.0) + fract(uv.x)/4.0;
	finalCoords.y = ((3.0-tile.y)/4.0) + fract(uv.y)/4.0;
	  
	finalCoords.y = 1.0-finalCoords.y;
	return textureGrad(texSampler, finalCoords, dFdx(uv/4.0), dFdy(uv/4.0));
	 
}
	
in vec4 fragColor;
in vec2 fragTexCoord;
uniform sampler2D tex;
uniform bool isBlock;

void main()
{
	
	if (isBlock) {
		gl_FragColor = wangSample(tex, fragTexCoord * vec2(4.0f, 4.0f));
		//gl_FragColor = texture2D(tex, fragTexCoord);		
		//gl_FragColor = texture2D(tex, fragTexCoord * vec2(1.0f, -1.0f));		
	} else {
		gl_FragColor = fragColor;
	}
}