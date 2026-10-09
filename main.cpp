#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

struct ScanPoint {
    float angle_deg;
    float distance_m;
};

class ObstacleAvoidanceSystem {
private:
    float safety_margin_m;

public:
    ObstacleAvoidanceSystem(float margin = 0.5f) : safety_margin_m(margin) {}

    std::string evaluatePath(const std::vector<ScanPoint>& scan_data) {
        float front_clearance = 100.0f;
        float left_clearance = 100.0f;
        float right_clearance = 100.0f;

        for (const auto& point : scan_data) {
            if (point.angle_deg >= -15.0f && point.angle_deg <= 15.0f) {
                front_clearance = std::min(front_clearance, point.distance_m);
            } else if (point.angle_deg > 15.0f && point.angle_deg <= 75.0f) {
                right_clearance = std::min(right_clearance, point.distance_m);
            } else if (point.angle_deg < -15.0f && point.angle_deg >= -75.0f) {
                left_clearance = std::min(left_clearance, point.distance_m);
            }
        }

        std::cout << "[Mesafe Analizi] On: " << front_clearance 
                  << "m | Sol: " << left_clearance 
                  << "m | Sag: " << right_clearance << "m" << std::endl;

        if (front_clearance > safety_margin_m) {
            return "DURUM: Guvenli | HAREKET: Ileri Devam Et";
        } else if (right_clearance > left_clearance && right_clearance > safety_margin_m) {
            return "DURUM: On Yol Tikali | HAREKET: Saga Don";
        } else if (left_clearance > safety_margin_m) {
            return "DURUM: On Yol Tikali | HAREKET: Sola Don";
        } else {
            return "DURUM: Kritik Engel | HAREKET: Dur ve Geri Yonel";
        }
    }
};

int main() {
    std::cout << "=== Otonom Engel Kacinma Sistemi Baslatildi ===" << std::endl;

    auto avoidance_module = std::make_unique<ObstacleAvoidanceSystem>(0.6f);

    std::vector<ScanPoint> lidar_readings = {
        {0.0f, 0.45f},
        {-45.0f, 1.20f},
        {45.0f, 0.80f}
    };

    std::string decision = avoidance_module->evaluatePath(lidar_readings);
    std::cout << decision << std::endl;

    return 0;
}