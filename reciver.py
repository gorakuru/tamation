#!/usr/bin/python
# coding: utf-8

import paho.mqtt.client as mqtt
import json
import requests

token = "token:token_*****"
remocon = "***.***.***.***"
remocon2 = "***.***.***.***"


class decodeErr(Exception):
	pass


def decodeRoom(r):
	if r in {'Living', 'living', u'リビング'}:
		return "Living"
	if r in {'Bedroom', 'bedroom', 'BedRoom', u'ベッドルーム', u'寝室'}:
		return "Bedroom"
	if r in {'Youshitu', 'youshitu','Youshitsu', 'youshitsu', u'洋室'}:
		return "Youshitu"
	print "room decode error"
	raise decodeErr()

def decodeDevice(d):
	if d in {'TV', 'tv', 'Tv', u'テレビ'}:
		return "TV"
	if d in {'Light', 'light', u'ライト', u'あかり', u'灯り', u'電気', u'明かり'}:
		return "Light"
	print "device decode error"
	raise decodeErr()

def decodeAction(a):
	if a in {'Power', 'power', u'電源', u'パワー'}:
		return "Power"
	if a in {'On', 'ON', 'on', u'オン'}:
		return "On"
	if a in {'Off', 'OFF', 'off', u'オフ'}:
		return "Off"
	if a in {'Button', 'button', 'ボタン', u'オン'}:
		return "Button"
	print "action decode error"
	raise decodeErr()

def router(room, device, action):
	if device == "TV" and action in {"On", "Off"}:
		return room + "TVPower"
	if room == "Bedroom" and device == "Light":
		return "BedroomLightButton"
	return room + device + action

def sendAction(record):
	try:
		room = decodeRoom(record['room'])
		device = decodeDevice(record['device'])
		action = decodeAction(record['action'])
		target = router(room, device, action)
	except decodeErr:
		return
	else:
		req = "http://"+remocon+"/?t="+target
		print(req)
		requests.get(req)
		req = "http://"+remocon2+"/?t="+target
		print(req)
		requests.get(req)


def on_connect(client, userdata, flags, respons_code):
    print('status {0}'.format(respons_code))
    client.subscribe("IFTTT/power", 1)

def on_message(client, data, msg):
    print(msg.topic + " " + (msg.payload).decode('utf_8') )
    j = json.loads((msg.payload))
    for r in j['data']:
    	sendAction(r)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(token)
client.connect("mqtt.beebotte.com", 1883, 60)

client.loop_forever()
