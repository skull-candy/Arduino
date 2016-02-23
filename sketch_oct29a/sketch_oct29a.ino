void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:


var pubnub = PUBNUB.init({
  publish_key: 'demo',
  subscribe_key: 'demo'
});
var channel = "c3-bar" + Math.random();
eon.chart({
  channel: channel,
  pubnub: pubnub,
  generate: {
    bindto: '#chart',
    data: {
      labels: true,
      type: 'bar'
    },
    bar: {
      width: {
        ratio: 0.5
      }
    },
    tooltip: {
        show: false
    }
  }
});
}
