#version 330 core
in vec2 TexCoords;     // e.g. (4.0, 3.0)
in vec4 TileBounds;    // e.g. (0.1, 0.1, 0.2, 0.2)
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D image;

// Simple directional lighting (same idea as your old simple shaders)
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main() {
    // 1. Local Tile Coordinates (0.0 to 1.0 repeating)
    vec2 t = fract(TexCoords);

    // 2. Map to Atlas Coordinates
    vec2 atlasUV = mix(TileBounds.xy, TileBounds.zw, t);

    // 3. Sample atlas
    vec4 texColor = texture(image, atlasUV);

    // Simple diffuse + ambient
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(-lightDir);
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 lit = ambientColor + diff * lightColor;

    // Apply lighting + vertex tint
    vec3 rgb = texColor.rgb * lit;
    FragColor = vec4(rgb, texColor.a);
}
