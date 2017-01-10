
 #ifdef GL_ES
 precision highp float;
 #endif
 
uniform sampler2D u_texSampler;

varying vec2 v_texCo;
varying vec3 v_ecNormal;
varying vec4 v_ecVert;

#define K_MAX_LIGHTS 16

//-------------------------
//  TMaterial
//-------------------------
struct TMaterial
{
    vec4    amb;
    vec4    dfs;
    vec4    spc;
    vec4    ems;
    float   shns;
    float   alpha;
};

//-------------------------
//  TLight
//-------------------------
struct TLight
{
    vec3    dir;
    vec3    pos;
    float   isPara;     // 1.0 para, 0.0 spot
    float   Kd2;        // atn = 1/(1 + Kd2 * dist^2)
    float   cosConeAngle;
    float   cosCutOff;
    TMaterial   mat;
};

//-------------------------
//  Uniform of Material/Light
//-------------------------
uniform TMaterial   u_material;
uniform TLight      u_lightAry[K_MAX_LIGHTS];
uniform int         u_lightNum;

//-------------------------
//  spotLightCoef
//-------------------------
float calcSpotLight(TLight light, vec3 L_spot)
{
    float a0 = light.cosConeAngle;
    float a1 = light.cosCutOff;
    
    vec3 L_dir = normalize(-light.dir);
    float ang = dot(L_spot, L_dir);
    float edge = max(0.0, (a0 - ang)/max(0.0001,  a0 - a1));
    float coef = clamp(mix(1.0, 0.0, edge), 0.0, 1.0);
//    coef = (ang > a1)? 1.0 : 0.0;
    coef = (light.isPara == 1.0)? 1.0 : coef;
    return coef;
}

//-------------------------
//  calcColor
//-------------------------
vec4 calcColor(TMaterial mtl, TLight light, vec3 ecNormal, vec4 ecVert)
{
    //--- Normal will be defected by scaling transform,
    // so a pre-calculated normalMatrix is provided
    // to bring the model normal into eye space.
    
    //---- Parallel Light Light
    vec3 L_dir = normalize(-light.dir);
    
    //---- Spot Light
    vec3 L_spot = normalize(light.pos - ecVert.xyz);
    float spotCoef = calcSpotLight(light, L_spot);
   
    //---- Check Parallel/Spot light
    vec3 L0 = (light.isPara == 1.0)? L_dir : L_spot;
    vec3 N = normalize(ecNormal);
    vec3 L = normalize(L0);
    vec3 E = vec3(0.0, 0.0, 1.0);
    vec3 H = normalize(L + E);
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, mtl.shns);
    
    TMaterial ml = light.mat;   // Light Material
    float d = distance(light.pos, ecVert.xyz);
    float li = 1.0/(1.0+ light.Kd2 * pow(d,2.0)) * spotCoef ;// Light Attenuetion
    vec4 c = mtl.amb * ml.amb * li + df * mtl.dfs * ml.dfs* li + sf * mtl.spc * ml.spc * li + mtl.ems;
    return c;
    
}



//-------------------------
//  main
//-------------------------
void main()
{
    vec4 c = vec4(0,0,0,1);
    for(int i = 0; i < u_lightNum; ++i)
        c += calcColor(u_material, u_lightAry[i], v_ecNormal, v_ecVert);
    
    vec4 cAlpha = vec4(c.rgb, u_material.alpha);
    vec4 cTex = texture2D(u_texSampler, v_texCo);
	gl_FragColor = cTex * cAlpha;
}