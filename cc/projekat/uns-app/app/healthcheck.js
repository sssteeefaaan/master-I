const http = require('http')
const env = process.env

const options = {
    host: '0.0.0.0',
    port: env["SERVER_PORT"] || 8000,
    timeout: 2000
};

const healthCheck = http.request(options, (res) => {
    console.log(`HEALTHCHECK STATUS: ${res.statusCode}`);
    if (res.statusCode < 400) {
        process.exit(0);
    }
    else {
        process.exit(1);
    }
});

healthCheck.on('error', function (err) {
    console.error('ERROR');
    process.exit(1);
});

healthCheck.end();