const fs = require("fs");

function processData(tickers) {
    var ans = {}
    const jsonString = fs.readFileSync("./rawData.json", "utf8")
    const data = JSON.parse(jsonString);
    for (const ticker of tickers) {
        const timeSeries = data[`${ticker}`]["Time_Series"]
        const timeSeriesData = []
        for (const val in timeSeries) {
            const data_point = {
                time: `${val}`,
                open: `${timeSeries[val].open}`,
                close: `${timeSeries[val].close}`,
                high: `${timeSeries[val].high}`,
                low: `${timeSeries[val].low}`,
                volume: `${timeSeries[val].volume}`
            }
            timeSeriesData.push(data_point)
        }
        
        timeSeriesData.reverse()

        ans[`${ticker}`] = {
            timeSeries: timeSeriesData
        }
    }

    return ans
}

const ret = processData(["AMZN", "GOOG"])

fs.writeFile('./data.json', JSON.stringify(ret), err => {
    if (err) {
      console.error(err)
      return
    }
});