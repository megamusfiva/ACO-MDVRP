# ACO-MDVRP.md
Berikut adalah Implementasi algoritma Ant Colony Optimization (ACO) untuk menyelesaikan permasalahan Multiple Depot Vehicle Routing Problem (MDVRP) dengan menggunakan Borland C++.

## Metode
+ Memberi nilai awal feromon τ_ij.
+ Menempatkan semua semut pada titik awal yang ditentukan secara acak.
+ Mengisi tabu list.
+ Kelompokkan obyek berdasarkan jarak minimum.
  + Menempatkan titik awal ke dalam tabu list. 
  + Menentukan titik berikutnya yang akan ditempatkan pada tabu list, proses dilakukan sampai semua titik masuk ke dalam tabu list.
    + Menghitung nilai probabilitas (pilih rute) dari titik awal ke titik yang akan dikunjungi.
    + Membangkitkan bilangan real r pada interval [0,1] ssecara acak untuk menentukan titik yang berikutnya berdasarkan nilai probabilitas pilih rute.
+ Evaluasi (menghitung panjang perjalanan) setiap semut dan menentukan solusi terbaik sementara.
+ Memperbaharui feromon berdasarkan pada penyelesaian yang diperoleh.
+ Cek iterasi. Apabila iterasi maksimum atau kondisi stagnan (semua semut memiliki perjalanan yang sama) belum terpenuhi, maka kosongkan tabu list dan kembali ke langkah 2. Apabila iterasi maksimum iterasi dan kondisi stagnan telah terpenuhi, maka iterasi berakhir.
+ Mencetak solusi sementara sebagai solusi terbaik yang didapatkan.

## Implementasi
Implementasinya ada di file ACO.CPP.

## Dataset
Terdapat 4 macam dataset yaitu :

* `DataD01M` : Data untuk perhitungan manual dengan jumlah pelanggan sebanyak 17 pelanggan, dan jumlah depot sebanyak 2 depot.
* `DataD01`  : Data kecil dengan jumlah pelanggan sebanyak 50 pelanggan, dan jumlah depot sebanyak 4 depot.
* `DataD02` : Data sedang dengan jumlah pelanggan sebanyak 75 pelanggan, dan jumlah depot sebanyak 5 depot.
* `DataD03` : Data besar dengan jumlah pelanggan sebanyak 99 pelanggan, dan jumlah depot sebanyak 2 depot.

## Deployment
File berikut harus berada pada satu folder :
```
ACO.CPP
DataD01.txt
DataD01M.txt
DataD02.txt
DataD03.txt
```

## Authors
* **Mega Musfivawati** - *Initial work* - [megamusfiva](https://github.com/megamusfiva/)
