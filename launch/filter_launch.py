from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # YAML dosyasının yolunu otomatik bul
    config = os.path.join(
        get_package_share_directory('lidar_filter_node'),
        'config',
        'params.yaml'
    )

    return LaunchDescription([
        Node(
            package='lidar_filter_node',
            executable='lidar_filter_node',
            name='lidar_filter_node',
            output='screen',
            parameters=[config]  # YAML dosyasını burada yüklüyoruz
        )
    ])

