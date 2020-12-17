#version 130

in vec3 vs_in_pos;
in vec3 vs_in_vel;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(vs_in_pos, 1);

	// �gy �rhat� fel�l a pontprimit�vek m�rete (felt�ve ha a kliens oldalon van egy glEnable(GL_PROGRAM_POINT_SIZE);)
	//gl_PointSize = 5;
}