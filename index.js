const fs = require('fs');
const http = require('http');
const url = require('url');
const devnull = require('dev-null');
const { spawn } = require('child_process');
const EventEmitter = require('events');

let routes = {};
routes['/'] = 'home.html';
routes['/home'] = 'home.html';
routes['/home.js'] = 'home.js';
routes['/home.css'] = 'home.css';
routes['/404'] = '404.html';
routes['/example1.txt'] = 'example1.txt';
routes['/example2.txt'] = 'example2.txt';
routes['/example3.txt'] = 'example3.txt';

const fileDir = './public/';

function getFile(res, path) {
    if (routes[path] === undefined) {
        path = '/404';
    }
    const file = fs.createReadStream(fileDir + routes[path]);
    file.pipe(res);
}

let postQueue = [];
function PostObject(req, res, path) {
    this.req = req;
    this.res = res;
    this.path = path;
}
let postEmitter = new EventEmitter();
postEmitter.on('next', function() {
    postQueue.shift();
    if (postQueue.length > 0) {
        postData(postQueue[0]);
    }
});

function postData(postObj) {
    if (postObj.path === '/render') {
        const child = spawn('./bin/render', ['-w'], { shell: true });
        postObj.req.pipe(child.stdin);
        child.stdout.pipe(postObj.res);
        child.stderr.pipe(devnull());

        child.on('exit', function(code, signal) {
            console.log('render.cpp completed with error code: ' + code);
            postEmitter.emit('next');
        });
    }
    else if (postObj.path === '/mutate') {
        const child = spawn('./bin/mutate', { shell: true });
        postObj.req.pipe(child.stdin);
        child.stdout.pipe(postObj.res);
        child.stderr.pipe(devnull());

        child.on('exit', function(code, signal) {
            console.log('mutate.cpp completed with error code: ' + code);
            postEmitter.emit('next');
        });
    }
    else if (postObj.path == '/cross') {
        const child = spawn('./bin/cross', ['-s'], { shell: true });
        postObj.req.pipe(child.stdin);
        child.stdout.pipe(postObj.res);
        child.stderr.pipe(devnull());

        child.on('exit', function(code, signal) {
            console.log('cross.cpp completed with error code: ' + code);
            postEmitter.emit('next');
        });
    }
    else {
        res.end();
        postEmitter.emit('next');
    }
}

function reqHandler(req, res) {
    let path = url.parse(req.url).path;
    console.log('Received request for: ' + path);
    
    if (req.method === 'GET') {
        getFile(res, path);
    }
    else if (req.method === 'POST') {
        postQueue.push(new PostObject(req, res, path));
        if (postQueue.length === 1) {
            postData(postQueue[0]);
        }
    }
    else {
        res.end();
    }
}

var server = http.createServer(reqHandler);

server.listen(8888);
console.log('Garden of Babel online at port 8888.');
