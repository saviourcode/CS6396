'use strict';
const request = require('request');
const fs = require('fs')

require('dotenv').config()

// replace the "demo" apikey below with your own key from https://www.alphavantage.co/support/#api-key
const url = `https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=IBM&interval=1min&apikey=${process.env.API_KEY}`;

request.get({
  url: url,
  json: true,
  headers: { 'User-Agent': 'request' }
}, (err, res, data) => {
  if (err) {
    console.log('Error:', err);
  } else if (res.statusCode !== 200) {
    console.log('Status:', res.statusCode);
  } else {
    // data is successfully parsed as a JSON object:

    fs.writeFile('./temp', JSON.stringify(data), err => {
      if (err) {
        console.error(err)
        return
      }
      //file written successfully
    })
  }
});