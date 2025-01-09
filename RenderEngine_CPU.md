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