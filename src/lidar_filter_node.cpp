#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"

// PCL Dönüşüm ve Filtre Kütüphaneleri
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>

class LidarFilterNode : public rclcpp::Node
{
public:
  LidarFilterNode() : Node("lidar_filter_node")
  {
    // --- YENİ: 1. Parametreleri Tanımlama (Varsayılan değerlerle) ---
    this->declare_parameter("voxel_leaf_size", 0.05);
    this->declare_parameter("sor_mean_k", 50);
    this->declare_parameter("sor_stddev_mul_thresh", 1.0);

    // Subscriber ve Publisher (kendi topiclerinize göre düzenleyiniz)
    subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
      "topic adınız", 10, std::bind(&LidarFilterNode::cloud_callback, this, std::placeholders::_1));

    publisher_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/velodyne_points_filtered", 10);

    RCLCPP_INFO(this->get_logger(), "Dinamik Parametreli Lidar Filtre Düğümü Başlatıldı.");
  }

private:
  void cloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
  {
    // --- YENİ: 2. Parametrelerin Anlık Değerlerini Okuma ---
    // Callback her tetiklendiğinde parametrelerin en güncel halini alır.
    double leaf_size = this->get_parameter("voxel_leaf_size").as_double();
    int mean_k = this->get_parameter("sor_mean_k").as_int();
    double stddev_thresh = this->get_parameter("sor_stddev_mul_thresh").as_double();

    // 1. ADIM: ROS 2 Mesajını PCL Veri Tipine Dönüştürme
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg(*msg, *cloud);

    if (cloud->empty()) {
        return;
    }

    // 2. ADIM: Voxel Grid Filtresi
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_voxel_filtered(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::VoxelGrid<pcl::PointXYZ> voxel_filter;
    voxel_filter.setInputCloud(cloud);
    
    // YENİ: Sabit değer yerine okuduğumuz dinamik leaf_size parametresini kullanıyoruz
    voxel_filter.setLeafSize(leaf_size, leaf_size, leaf_size); 
    voxel_filter.filter(*cloud_voxel_filtered);

    // 3. ADIM: Statistical Outlier Removal
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_final_filtered(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor_filter;
    sor_filter.setInputCloud(cloud_voxel_filtered);
    
    // YENİ: Sabit değerler yerine dinamik parametreleri kullanıyoruz
    sor_filter.setMeanK(mean_k); 
    sor_filter.setStddevMulThresh(stddev_thresh); 
    sor_filter.filter(*cloud_final_filtered);

    // 4. ADIM: Tekrar ROS 2 Mesajına Dönüştürme
    sensor_msgs::msg::PointCloud2 output_msg;
    pcl::toROSMsg(*cloud_final_filtered, output_msg);
    output_msg.header = msg->header;

    // 5. ADIM: Yayınlama
    publisher_->publish(output_msg);
    
    // Log ekranında parametrelerin aktif değerlerini de görelim
    RCLCPP_INFO(this->get_logger(), "Nokta: %lu -> %lu", 
                cloud->size(), cloud_final_filtered->size());
  }

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LidarFilterNode>());
  rclcpp::shutdown();
  return 0;
}
