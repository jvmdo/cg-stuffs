uniform sampler2D texture;
uniform vec2 pixelSize;	//1.0/640.0, 1.0/480.0

void main()
{
	vec2 pos=gl_FragCoord.xy*pixelSize;
	vec4 result;
	result+=1.0*texture2D(texture,vec2(pos.x-pixelSize.x,pos.y-pixelSize.y));
	result+=2.0*texture2D(texture,vec2(pos.x,pos.y-pixelSize.y));
	result+=1.0*texture2D(texture,vec2(pos.x+pixelSize.x,pos.y-pixelSize.y));
	result+=2.0*texture2D(texture,vec2(pos.x-pixelSize.x,pos.y));
	result+=3.0*texture2D(texture,vec2(pos.x,pos.y));
	result+=2.0*texture2D(texture,vec2(pos.x+pixelSize.x,pos.y));
	result+=1.0*texture2D(texture,vec2(pos.x-pixelSize.x,pos.y+pixelSize.y));
	result+=2.0*texture2D(texture,vec2(pos.x,pos.y+pixelSize.y));
	result+=1.0*texture2D(texture,vec2(pos.x+pixelSize.x,pos.y+pixelSize.y));
	result/=15.0;
	gl_FragColor=vec4(result.rgb,1.0);
	
}
