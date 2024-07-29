#version 330 core
layout (location=0) in vec3 position;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform mat4 m_matrix;            // 这些是分开的模型和视图矩阵
uniform mat4 v_matrix;
uniform float tf;                 // 用于动画和放置立方体的时间因子

out vec4 varyingColor;

mat4 buildRotateX(float rad);     // 矩阵变换工具函数的声明
mat4 buildRotateY(float rad);     // GLSL 要求函数先声明后调用
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void){
//    float i = gl_InstanceID + tf;   // 取值基于时间因子，但是对每个立方体示例也都是不同的
//    float a = sin(2.0 * i) * 8.0;   // 这些是用来平移的 x、y、z 分量
//    float b = sin(3.0 * i) * 8.0;
//    float c = sin(4.0 * i) * 8.0;
//    // 构建旋转和平移矩阵，将会应用于当前立方体的模型矩阵
//    mat4 localRotX = buildRotateX(1000*i);
//    mat4 localRotY = buildRotateY(1000*i);
//    mat4 localRotZ = buildRotateZ(1000*i);
//    mat4 localTrans = buildTranslate(a,b,c);
//    // 构建模型矩阵，然后是模型-视图矩阵
//    mat4 newM_matrix = m_matrix * localTrans * localRotX * localRotY * localRotZ;
//    mat4 mv_matrix = v_matrix * newM_matrix;
    
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
    varyingColor = vec4(position,1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

// 构建并返回平移矩阵
mat4 buildTranslate(float x, float y, float z){
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, 
                    0.0, 1.0, 0.0, 0.0, 
                    0.0, 0.0, 1.0, 0.0, 
                    x, y, z, 1.0 ); 
    return trans;
} 

// 构建并返回绕X轴的旋转矩阵
mat4 buildRotateX(float rad){
    mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0, 
                   0.0, cos(rad), -sin(rad), 0.0, 
                   0.0, sin(rad), cos(rad), 0.0, 
                   0.0, 0.0, 0.0, 1.0 ); 
    return xrot;
}

// 构建并返回绕Y轴的旋转矩阵
mat4 buildRotateY(float rad){
    mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0, 
                   0.0, 1.0, 0.0, 0.0, 
                   -sin(rad), 0.0, cos(rad), 0.0, 
                   0.0, 0.0, 0.0, 1.0 ); 
    return yrot;
}

// 构建并返回绕Z轴的旋转矩阵
mat4 buildRotateZ(float rad){
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0, 
                   sin(rad), cos(rad), 0.0, 0.0, 
                   0.0, 0.0, 1.0, 0.0, 
                   0.0, 0.0, 0.0, 1.0 ); 
    return zrot;
}

// 构建并返回缩放矩阵
mat4 buildScale(float x, float y, float z){
    mat4 scale = mat4(x, 0.0, 0.0, 0.0, 
                    0.0, y, 0.0, 0.0, 
                    0.0, 0.0, z, 0.0, 
                    0.0, 0.0, 0.0, 1.0 ); 
    return scale;
}