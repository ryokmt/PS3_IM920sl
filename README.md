# PS3_IM920sl
IM920slをBrain BoardのSBDBTソケットに刺せるライブラリ。

https://www.interplan.co.jp/solution/wireless/im920sl/　・・・IM920sl本体データシート
https://www.interplan.co.jp/solution/wireless/others/im920c_adp.php　・・・26ピン変換基盤データシート

フォルダ説明:
　・PS3_IM920sl_TXは、arduino UnoにUSB_Host_Shield、コントローラーを有線で接続する。IM920slは、26ピンに変換し、UART接続をすること。
     ピン配置は以下のデータシートから参照：https://www.interplan.co.jp/solution/wireless/others/im920c_adp.php
 
  ・PS3_IM920sl_RXは、mbed用ライブラリ。＊＊PS3_SBDBTと同時使用不可＊＊。使わない方のライブラリをすべてコメントアウトすること。
  
mbedライブラリ使用方法:
　基本的にPS3_SBDBTの宣言部分を変えるだけ。
 
 /*-----------------------------------------*/
 PS3_IM920sl ps3(PA_0, PA_1);//Rx, Tx
 /*-----------------------------------------*/
 
 データが保存される変数もすべてSBDBTライブラリを使用するときと同じ。"ps3.変数" で呼び出せる。
 
注意:
  ・IM920slの仕様上、60ms待たないと次のデータが送れないため、普通にのコントローラーの値を受け取るよりも低速
  ・Rx側とTx側のIM920slのボーレートは別々であるため、取り外して交換する場合は、Rx側を9600Hz、Tx側を11520Hzに設定すること。変更方法はデータシート参照。
