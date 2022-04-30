const express = require('express')
const router = express.Router()
const dataStore = require('../data/data.json')
var i = 0
router.get('/:ticker', function (req, res){
    const ticker = req.params.ticker
    dataStore[`${ticker}`]["timeSeries"][i].time =  new Date().toLocaleString();
    res.send(
        dataStore[`${ticker}`]["timeSeries"][i++]
    )
})

module.exports = router