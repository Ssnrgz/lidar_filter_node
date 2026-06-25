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
```bash
ros2 param set /lidar_filter_node voxel_leaf_size 0.10
