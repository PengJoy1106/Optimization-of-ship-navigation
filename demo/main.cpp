#include <iostream>
#include <cmath>


constexpr double kEarthRadiusKm = 6371.0; // ����뾶�����
constexpr double kPi = 3.14159265358979323846; // �� �Ľ���ֵ

double degreesToRadians(double degrees) {
    return degrees * kPi / 180.0;
}

double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    // ����γ�ȴӽǶ�ת��Ϊ����
    lat1 = degreesToRadians(lat1);
    lon1 = degreesToRadians(lon1);
    lat2 = degreesToRadians(lat2);
    lon2 = degreesToRadians(lon2);

    // �����ֵ
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    // Ӧ�� Haversine ��ʽ
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon / 2) * std::sin(dlon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    double distance = kEarthRadiusKm * c;
    return distance;
}

int main() {

    // ����ʡ�Ϸ��еľ�γ��
//    double latAnhui = 31.8639; // γ��
//    double lonAnhui = 117.2808; // ����
    // ʾ�����꣺ŦԼ�к���ɼ�ľ�γ��
    double latNYC = 40.7128; // γ��
    double lonNYC = -74.0060; // ����
    // ����ʡ�Ͼ��еľ�γ��
    double latJiangsu = 32.0603; // γ��
    double lonJiangsu = 118.7969; // ����

    // �������
    double distance = haversineDistance(latNYC, lonNYC, latJiangsu, lonJiangsu);

    std::cout <<" ����֮��ľ���Ϊ " << distance << " ����" << std::endl;

    return 0;
}
