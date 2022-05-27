uniform sampler2D texture;
uniform vec2 pixelSize;	//1.0/640.0, 1.0/480.0
varying vec2 outUV;

void main()
{
	vec2 pos=gl_FragCoord.xy*pixelSize;   //0..640,0..480 => 0..1	
	gl_FragColor=texture2D(texture,pos);
}
