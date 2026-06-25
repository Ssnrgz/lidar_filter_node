# lidar_filter_node
ROS 2 Humble tabanlı, PCL (Point Cloud Library) kullanarak ham LiDAR verilerini işleyen modüler filtreleme düğümü.

## Özellikler
* **Voxel Grid Filtresi:** Nokta bulutu çözünürlüğünü optimize eder (Downsampling).
* **Statistical Outlier Removal:** Havada asılı kalan sahte gürültüleri temizler.
* **YAML & Launch Desteği:** Parametreler yeniden derleme gerektirmeden dinamik olarak değiştirilebilir.
