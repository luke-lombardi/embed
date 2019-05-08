from pubnub.pnconfiguration import PNConfiguration
from pubnub.exceptions import PubNubException
from pubnub.pubnub import PubNub
 
pnconfig = PNConfiguration()
pnconfig.subscribe_key = "sub-c-41a2f9e6-6e96-11e9-8724-8269f6864ada"
pnconfig.publish_key = "pub-c-030e0611-2f65-49db-b496-29f6f0ff25f9"
pnconfig.ssl = False
 
pubnub = PubNub(pnconfig)

CHANNEL_NAME = "test_channel"

payload = {
  'a': 1,
  'b': 2
}

try:
  envelope = pubnub.publish().channel(CHANNEL_NAME).message(payload).sync()
  print("publish timetoken: %d" % envelope.result.timetoken)
except PubNubException as e:
  handle_exception(e)