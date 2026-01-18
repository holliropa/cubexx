#version 330 core
in vec3 Normal;
in vec2 TryTestCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(-lightDir);

    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 texColor = texture(texture1, TryTestCoords);
    vec3 result = (ambientColor + diffuse) * texColor.rgb;

    FragColor = vec4(result.rgb, texColor.a);
}