const fs = require('fs');
const http = require('http');
const url = require('url');
const devnull = require('dev-null');
const { spawn } = require('child_process');

let routes = {};
routes['/'] = 'home.html';
routes['/home'] = 'home.html';
routes['/home.js'] = 'home.js';
routes['/home.css'] = 'home.css';
routes['/404'] = '404.html';

const fileDir = './public/';

function getFile(res, path) {
    if (routes[path] === undefined) {
        path = '/404';
    }
    const file = fs.createReadStream(fileDir + routes[path]);
    file.pipe(res);
}

function postData(req, res, path) {
    if (path === '/render') {
        const child = spawn('./bin/render', ['-w'], { shell: true });
        req.pipe(child.stdin);
        child.stdout.pipe(res);
        child.stderr.pipe(devnull());

        child.on('exit', function(code, signal) {
            console.log('render.cpp completed with error code: ' + code);
        });
    }
    else if (path === '/mutate') {
        const child = spawn('./bin/mutate', { shell: true });
        req.pipe(child.stdin);
        child.stdout.pipe(res);
        child.stderr.pipe(devnull());

        child.on('exit', function(code, signal) {
            console.log('mutate.cpp completed with error code: ' + code);
        });
    }
    else if (path == '/cross') {
        const child = spawn('./bin/cross', ['-s'], { shell: true });
        req.pipe(child.stdin);
        child.stdout.pipe(res);
        child.stderr.pipe(devnull());

        child.on('exit', function(code, signal) {
            console.log('cross.cpp completed with error code: ' + code);
        });
    }
    else {
        res.end();
    }
}

function reqHandler(req, res) {
    let path = url.parse(req.url).path;
    console.log('Received request for: ' + path);
    
    if (req.method === 'GET') {
        getFile(res, path);
    }
    else if (req.method === 'POST') {
        postData(req, res, path);
    }
    else {
        res.end();
    }
}

var server = http.createServer(reqHandler);

server.listen(8888);
console.log('Garden of Babel online at port 8888.');
