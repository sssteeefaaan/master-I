let requests = 0

function getCount(){
    return requests
}

function newRequest(req, res, next){
    const hostname = req.hostname
    const path = req.path
    const timestamp = new Date(Date.now()).toISOString()
    const method = req.method
    console.log(`(${ requests++ })\t [${timestamp}]\t Hostname(${hostname})\t Method(${ method })\t Path(${path})`)
    next()
}

module.exports = {getCount, newRequest}