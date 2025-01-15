# 1.绘制一个三角形
**思路**:遍历三角形三个顶点所构成的包围盒中的所有像素,根据重心坐标判断点是否在三角形内
**数学推导**:一个点P对于三角形ABC的重心坐标公式：$P=(1-u-v)A+uB+vC$
我们可以把它转换成以A为原点,AB,AC向量为轴的表达式,只要两边同时减A:
$$P-A=(1-u-v)A-A+uB-A+vC-A$$
$$\Rightarrow P=A+u\overrightarrow{AB}+v\overrightarrow{AC}$$
再变形一下:$\overrightarrow{PA}+u\overrightarrow{AB}+v\overrightarrow{AC}=0$
把上面的式子拆一下:
$$D(x) = \begin{cases}
u\overrightarrow{AB_{x}}+v\overrightarrow{AC_{x}}+\overrightarrow{PA_{x}}=0\\
u\overrightarrow{AB_{y}}+v\overrightarrow{AC_{y}}+\overrightarrow{PA_{y}}=0\\
\end{cases}$$
由此,我们可以得到两个矩阵乘法等式:
$$\begin{bmatrix}u & v & 1 \\\end{bmatrix}\begin{bmatrix}\overrightarrow{AB_{x}} \\ \overrightarrow{AC_{x}} \\ \overrightarrow{PA_{x}} \\\end{bmatrix}=0$$
$$\begin{bmatrix}u & v & 1 \\\end{bmatrix}\begin{bmatrix}\overrightarrow{AB_{y}} \\ \overrightarrow{AC_{y}} \\ \overrightarrow{PA_{y}} \\\end{bmatrix}=0$$
可以看到向量$\begin{bmatrix}u & v & 1 \\\end{bmatrix}$点乘$\begin{bmatrix}\overrightarrow{AB_{x}} & \overrightarrow{AC_{x}} & \overrightarrow{PA_{x}}\\\end{bmatrix}$和$\begin{bmatrix}\overrightarrow{AB_{y}} & \overrightarrow{AC_{y}} & \overrightarrow{PA_{y}}\\\end{bmatrix}$的结果都是0,所以向量$\begin{bmatrix}u & v & 1 \\\end{bmatrix}$垂直于上面两个向量组成的平面,也是就是说$\begin{bmatrix}\overrightarrow{AB_{x}} & \overrightarrow{AC_{x}} & \overrightarrow{PA_{x}}\\\end{bmatrix}$和$\begin{bmatrix}\overrightarrow{AB_{y}} & \overrightarrow{AC_{y}} & \overrightarrow{PA_{y}}\\\end{bmatrix}$叉乘的结果是一个平行于$\begin{bmatrix}u & v & 1 \\\end{bmatrix}$的向量.假设计算出的向量是$\begin{bmatrix}e & f & g \\\end{bmatrix}$,要想求出u, v的值需要同时除以g.也就是$u=e/g$, $v=f/g$, $1=g/g$这也要求$g$不可以等于0.
那么$g$的几何意义是什么呢?根据向量叉乘的行列式计算$g=\begin{vmatrix}\overrightarrow{AB_{x}} & \overrightarrow{AC_{x}} \\\overrightarrow{AB_{y}} & \overrightarrow{AC_{y}} \end{vmatrix}$也就是说$g$等于$\overrightarrow{AB}$和$\overrightarrow{AC}$叉乘的结果如果$g$等于0那么三角形退化为一条线段.由于所有的点都是整数坐标,所以叉乘的结果小于1就可以视为三角形退化.
**代码实现**
```c
Vec3f barycentric(const Vec2i *pts, const Vec2i P) {  
    const Vec3i u = Vec3i(pts[1].x-pts[0].x,pts[2].x-pts[0].x,pts[0].x-P.x)
    ^ Vec3i(pts[1].y-pts[0].y,pts[2].y-pts[0].y,pts[0].y-P.y);  
    if (std::abs(u.z) < 1) return {-1, 1, 1};  
    return {1.f-static_cast<float>(u.x + u.y)/static_cast<float>(u.z),
    static_cast<float>(u.x) / static_cast<float>(u.z),
    static_cast<float>(u.y) / static_cast<float>(u.z)};  
}
```
# 2.Z-Buffer
为了处理三角形相互遮挡问题引入zbuffer.有一种算法叫画家算法用来解决遮挡关系,画家算法的核心思想是从后向前绘制对象,这在一些UI的渲染中没有问题,但是一旦出现三角形相互遮挡的情况画家算法就无法解决,所以需要引入zbuffer.

 zbuffer的思路也很简单,就是计算每个像素的深度.然后在绘制三角形的时候查看要绘制的像素点的深度,如果深度大于记录深度就绘制并把最新深度记录下来.
 
 一个三角形绘制到屏幕上的各个像素的深度可以由重心坐标求出来.先计算出这个点的重心坐标$\begin{bmatrix}u & v & w \\\end{bmatrix}$,然后做三个点z值的插值$A.z\cdot u+B.z\cdot u+C.z\cdot u$,注意这三个点是转换成屏幕坐标的三个点.

**uv**:获取一个点的uv坐标同理,也是使用重心坐标$uv=uv_{0}\cdot u+uv_{1}\cdot v+uv_{2}\cdot w$

# 3.View/Camera Transform
*对应资料:games101_lecture_4  tiny_render_lesson4*

视图变换矩阵$M_{view}=R_{view}T_{view}$, $R_{view}$是旋转矩阵 $T_{view}$是位移矩阵
因为相机和物体相对位置不变则渲染结果不变,那么把相机放在一个标准位置上.相机移动到标准位置后,所有物体要随着相机移动.先位移相机再旋转相机.相机的观测方向是$\hat g$,向上方向是$\hat t$.

相机的标准位置是:坐标是$(0,0,0)$,$\hat g$与$-z$重合,$\hat t$与$y$重合

==旋转矩阵是正交矩阵,旋转矩阵的逆等于旋转矩阵的转置矩阵==
由于求将相机旋转到坐标轴的旋转矩阵$R$很难求所以我们可以先求出将坐标轴旋转成相机的角度的矩阵,也就是转转矩阵的逆矩阵$R^{-1}$
$$R_{view}^{-1}=\begin{bmatrix}x_{\hat g \times \hat t}&x_{t}&x_{-g}&0 \\ y_{\hat g \times \hat t}&y_{t}&y_{-g}&0 \\ z_{\hat g \times \hat t}&z_{t}&z_{-g} &0 \\ 0&0&0&1\\\end{bmatrix} \Rightarrow R_{view}=\begin{bmatrix}x_{\hat g \times \hat t}&y_{\hat g \times \hat t}&z_{\hat g \times \hat t}&0 \\ x_{t}&y_{t}&z_{t}&0 \\ x_{-g}&y_{-g}&z_{-g} &0 \\ 0&0&0&1\\\end{bmatrix}$$
为什么上述推导成立?或者$R^{-1}$是怎么得出来的?
$R^{-1}$就是坐标轴旋转成相机角度的矩阵,比如$x$轴$(1,0,0,0)$,因为是齐次坐标向量所以最后一个分量是0,$R^{-1}\times x$得到的向量的方向就是是$\hat g \times \hat t$的方向,其他三个轴同理.所以$$R_{view}^{-1}=R_{view}^{T} \Rightarrow R_{view}=(R_{view}^{-1})^{T}$$
## 3.1正交投影 Orthogonal Projection
正交投影把空间中的一个立方体区域$[l,r]\times[b,t]\times[f,n]$(以上字母分别是x,y,z轴上的范围)移动到原点并压缩成一个标准立方体(canonical cube)$[-1,1]^{3}$,将立方体转成标准立方体的矩阵是$M_{ortho}$
$$M_{ortho}=\begin{bmatrix}
\frac{2}{r-l}&0&0&0 \\
0&\frac{2}{t-b}&0&0 \\
0&0&\frac{2}{n-f}&0 \\
0&0&0&1\\\end{bmatrix}
\begin{bmatrix}
1&0&0&-\frac{r+l}{2} \\
0&1&0&-\frac{t+b}{2} \\
0&0&1&-\frac{n+f}{2} \\
0&0&0&1 \\\end{bmatrix}$$

## 3.2