# lidar_filter_node
ROS 2 Humble tabanlı, PCL (Point Cloud Library) kullanarak ham LiDAR verilerini (`sensor_msgs/msg/PointCloud2`) işleyen modüler ve dinamik bir filtreleme düğümü.

## Özellikler
* **Voxel Grid Filtresi:** Nokta bulutu çözünürlüğünü optimize eder (Downsampling).
* **Statistical Outlier Removal:** Havada asılı kalan sahte gürültüleri temizler.
* **YAML & Launch Desteği:** Parametreler yeniden derleme gerektirmeden harici dosyadan yüklenir ve runtime esnasında dinamik olarak değiştirilebilir.

---

## Parametre Açıklamaları (`config/params.yaml`)

### 1. `voxel_leaf_size: 0.05` (Voxel Grid Filtresi)
* **Anlamı:** Nokta bulutunu seyrelteceğimiz 3 boyutlu küplerin (voxel) metre cinsinden kenar uzunluğudur (5 cm x 5 cm x 5 cm).
* **Görevi:** Bu 5 cm'lik küpün içine düşen onlarca LiDAR noktasının ortalamasını (centroid) alarak tek bir noktaya indirger.
* **Nasıl Değiştirilir?** * Eğer simülasyon veya işlemci (CPU) çok kasılıyorsa bu değeri büyütüp (örn: `0.10` yani 10 cm) nokta sayısını azaltabilirsin.
  * Eğer engelleri çok daha hassas görmek istiyorsan değeri küçültebilirsin (örn: `0.02` yani 2 cm).

### 2. `sor_mean_k: 50` (Statistical Outlier Removal - Gürültü Filtresi)
* **Anlamı:** Analiz edilecek komşu nokta sayısıdır.
* **Görevi:** Algoritma, nokta bulutundaki her bir noktanın en yakınındaki 50 komşusunu bulur ve bu komşulara olan ortalama mesafesini hesaplar.
* **Nasıl Değiştirilir?** Genellikle 30 ile 50 arasında bırakılır. Değeri çok artırmak işlem yükünü çoğaltır, çok düşürmek ise gürültü analizini verimsizleştirir.

### 3. `sor_stddev_mul_thresh: 1.0` (Standart Sapma Çarpan Eşiği)
* **Anlamı:** Bir noktanın "gürültü" (outlier) olarak kabul edilip silinmesi için gereken istatistiksel mesafe eşiğidir.
* **Görevi:** Bir noktanın komşularına olan mesafesi, genel ortalamanın standart sapmasından (bu çarpanla çarpılmış halinden) büyükse o nokta havada uçuşan sahte bir gürültü sayılır ve silinir.
* **Nasıl Değiştirilir?**
  * **Değeri düşürürsen (örn: `0.5`):** Filtre çok agresifleşir, en ufak bir uzakta kalmış noktayı bile siler (gerçek nesne noktalarını da silebilir).
  * **Değeri yükseltirsen (örn: `2.0`):** Filtre daha esnek olur, sadece gruptan çok uzakta kalmış bariz sahte noktaları temizler.

---

## Çalıştırma ve Dinamik Kalibrasyon

Launch dosyası çalıştığında düğüm doğrudan bu `0.05`, `50` ve `1.0` değerlerini baz alarak filtrelemeye başlayacaktır:

```bash
ros2 launch lidar_filter_node filter_launch.py

Eğer simülasyon veya test esnasında parametreleri anlık (runtime) olarak değiştirmek istersen şu komutu kullanabilirsin:

ros2 param set /lidar_filter_node voxel_leaf_size 0.10
```
------------------------------------------------------------------------------------------

# lidar_filter_node

A modular and dynamic ROS 2 Humble node that processes raw LiDAR data (`sensor_msgs/msg/PointCloud2`) using the Point Cloud Library (PCL).

## Features
* **Voxel Grid Filter:** Optimizes point cloud density and resolution (Downsampling).
* **Statistical Outlier Removal:** Filters out sparse, floating noise and artifacts.
* **YAML & Launch Support:** Parameters are loaded via an external configuration file and can be dynamically modified at runtime without rebuilding.

---

## Parameter Descriptions (`config/params.yaml`)

### 1. `voxel_leaf_size: 0.05` (Voxel Grid Filter)
* **Meaning:** The side length of the 3D cubes (voxels) used to downsample the point cloud, measured in meters (5 cm x 5 cm x 5 cm).
* **Function:** It averages the coordinates of all LiDAR points falling into a single 5 cm cube and reduces them to a single centroid point.
* **How to Tune:**
  * If the simulation or your CPU performance drops significantly, increase this value (e.g., `0.10` or 10 cm) to decrease the point count.
  * If you need high-precision obstacle detection, decrease this value (e.g., `0.02` or 2 cm).

### 2. `sor_mean_k: 50` (Statistical Outlier Removal - Noise Filter)
* **Meaning:** The number of nearest neighbors to analyze for each point.
* **Function:** The algorithm computes the average distance from any given point to its 50 closest neighbors.
* **How to Tune:** It is generally kept between 30 and 50. Setting it too high increases the computational overhead, while setting it too low makes the noise analysis less accurate.

### 3. `sor_stddev_mul_thresh: 1.0` (Standard Deviation Multiplier Threshold)
* **Meaning:** The statistical distance threshold that determines whether a point is classified as noise (outlier).
* **Function:** If a point's distance to its neighbors is greater than the global average distance plus the standard deviation (scaled by this multiplier), it is considered floating noise and is removed.
* **How to Tune:**
  * **Lowering the value (e.g., `0.5`):** Makes the filter more aggressive, eliminating slightly isolated points (which might occasionally include real object points).
  * **Increasing the value (e.g., `2.0`):** Makes the filter more lenient, removing only highly isolated, obvious artifacts.

---

## Deployment and Dynamic Calibration

When the launch file runs, the node initializes and begins filtering using the default `0.05`, `50`, and `1.0` values:

```bash
ros2 launch lidar_filter_node filter_launch.py

To adjust parameters on the fly (runtime) during a simulation or a live test, use the following command:

ros2 param set /lidar_filter_node voxel_leaf_size 0.10
```
