#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec3 matambF;
out vec3 matdiffF;
out vec3 matspecF;
out float matshinF;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 posFocus;

uniform int escena;
out vec4 vertSCO;
out vec3 NormSCO;

//out vec3 fcolor;

void main()
{	

    // Passar vector normal a SCO
	mat3 NormalMatrix = inverse(transpose(mat3(view*TG)));
    NormSCO = NormalMatrix*normal; //vector normal a SCO
    
    //Deixar posFocus en SCA
    if (escena == 1) vec4 posFSCO = view*vec4(posFocus, 1.0); 


    vertSCO = view*TG*vec4(vertex, 1.0); //vertexs a SCO
    //Asignar valores para pasar a frag shader
    matambF = matamb;
	  matdiffF = matdiff;
	  matspecF = matspec;
	  matshinF = matshin;
	
    //Lambert:
    //fcolor = Ambient() + Difus(NormSCO, L, colFocus); 
    //Phong:
    //fcolor = Ambient() + Difus(NormSCO, L, colFocus)+Especular(NormSCO,L,vertSCO,colFocus); 

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
