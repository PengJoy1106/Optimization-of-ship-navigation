# 基本原理
‌‌‌　　计算两个地球表面点之间的距离需要使用大圆距离公式（Haversine formula）（Haversine公式假设地球是一个完美的球体，而实际上地球的形状更接近于椭球体，所以在极端情况下，Haversine公式的结果可能会有一些误差。对于更高精度的距离计算，可以使用更复杂的方法，如Vincenty公式。）。
‌‌‌　　当计算两个地球上的经纬度点之间的距离时，我们需要考虑地球的曲率，因为地球不是平面而是一个球体。Haversine公式是一种用于计算球面上两点间距离的方法，它基于球面三角学。公式的核心思想是使用球面三角形的一个特殊情况，即两点之间的弦长。Haversine公式通过计算这两个点的经纬度差值，并应用一些三角学关系，来计算出两点之间的弦长，从而得到球面距离。
Haversine公式的步骤：
1. 将经纬度从角度转换为弧度：由于三角函数通常使用弧度而不是角度进行计算，我们需要将经纬度从角度转换为弧度。这可以通过将角度乘以 π / 180 来实现。
2. 计算两点的差值：计算出纬度和经度的差值，即 Δlat 和 Δlon。
3. 应用Haversine公式：Haversine公式的数学表示为 haversin(θ) = sin²(θ/2)，其中θ 是两点之间的中心角。公式中涉及 sin、cos 和 atan2 等三角函数。
4. 计算中心角：通过计算 Δlat 和 Δlon 的一些组合，可以得到中心角θ。
5. 计算球面距离：使用 haversin(θ) 来计算弦长，然后乘以地球半径（通常以公里或英里为单位），即可得到球面上两点之间的距离。
# 示例代码
```
#include <iostream>
#include <cmath>

constexpr double kEarthRadiusKm = 6371.0; // 地球半径（公里）
constexpr double kPi = 3.14159265358979323846; // π 的近似值

double degreesToRadians(double degrees) {
    return degrees * kPi / 180.0;
}

double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    // 将经纬度从角度转换为弧度
    lat1 = degreesToRadians(lat1);
    lon1 = degreesToRadians(lon1);
    lat2 = degreesToRadians(lat2);
    lon2 = degreesToRadians(lon2);

    // 计算差值
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    // 应用 Haversine 公式
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon / 2) * std::sin(dlon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    double distance = kEarthRadiusKm * c;
    return distance;
}

int main() {
    // 安徽省合肥市的经纬度
    double latAnhui = 31.8639; // 纬度
    double lonAnhui = 117.2808; // 经度
    // 江苏省南京市的经纬度
    double latJiangsu = 32.0603; // 纬度
    double lonJiangsu = 118.7969; // 经度

    // 计算距离
    double distance = haversineDistance(latAnhui, lonAnhui, latJiangsu, lonJiangsu);

    std::cout << "安徽省合肥市与江苏省南京市之间的距离为 " << distance << " 公里" << std::endl;

    return 0;
}
 ```
# 测试结果
在Visual Studio上编写代码并进行编译输出结果如下图所示
 ![image.png|500](https://blog-1310087999.cos.ap-nanjing.myqcloud.com/20230823204958.png)


