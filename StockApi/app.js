const express = require('express')
const app = express()
const stockRoutes = require('./routes/stock')

app.use('/stock', stockRoutes)

app.listen(process.env.PORT || 8080)