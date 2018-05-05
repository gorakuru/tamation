# tamation
田村家の赤外線リモコン用
ESPr IR赤外線リモコンを使って家電を制御

# ゴール 

1. Google home mini -> IFTTT -> MQTT(beebotte) -> raspberry Pi -> mqtt -> ESPr IRリモコン -> 照明とか
1. (HTTP) -> WEBサーバー -> MQTT(beebotte) -> raspberry Pi -> mqtt -> ESPr IRリモコン -> 照明とか

各部屋にESPrを置くので、RPIから ESP間はmqttで。各部屋に置くESPrは全く同じソースコードでOK。


# 現状

Google home mini -> IFTTT -> MQTT(beebotte) -> raspberry Pi -> mqtt -> ESPr IRリモコン -> 照明とか

とりあえず、ESPr IRリモコンの中のコードからはリモコンコードやコマンドリストなどを排除できたので、あとは学習する方法を何とかしたい。

# リンク
* ESPr IR 赤外線リモコン：http://trac.switch-science.com/wiki/ESP-IR
* RAWで赤外線を送信する方法：https://jiwashin.blogspot.jp/2016/09/ir-remote-control-of-air-conditioner-with-esp8266.html
* mqttとRESTが使える無料のメッセージングサービス : https://beebotte.com
* beebotteとIFTTTの組み合わせ : https://qiita.com/mayfair/items/e761c788a9d8787bc610

# 使い方

## ファイルの説明
* irsend/irsend.ino : ESPr用のスケッチファイル
* reciver.py : rpi上で動かす、IFTTTからmqtt経由でコマンドをもらい、ESPrがsubscribeしているmqttにpublish

## リモコンデータの取得方法
IRremoteESP8266というライブラリのサンプルスケッチ：IRrecvDumpを実行し、rawデータを得る。
この時ESPr IR 赤外線リモコンの場合、READするピンは5になる。
