const inputElements = document.querySelectorAll('#inputPane textarea');
const outputContainers = document.querySelectorAll('.outputContainer');
const genomeButtons = document.querySelectorAll('#genomeSelect div');
const operationButtons = document.querySelectorAll('#operationSelect button');
const alertBox = document.querySelector('#alertBox');
const helpContainer = document.querySelector('#helpContainer');
const loadExample = document.querySelector('#loadExample');

function makeRequest(method, path, requestText, onResponse) {
    let req = new XMLHttpRequest();
    req.open(method, path, true);
    req.onreadystatechange = function() {
        if (req.readyState !== 4 || req.status !== 200) { return; }
        onResponse(req.responseText);
    };
    req.send(requestText);
}

let timer;
let elapsed;
function makeAlert(message) {
    window.clearTimeout(timer);
    elapsed = 0;
    alertBox.style.opacity = 1;
    alertBox.innerHTML = message;
    (function fade() {
        elapsed++;
        if (elapsed > 60) {
            alertBox.style.opacity -= 0.1;
        }
        if (alertBox.style.opacity > 0) {
            timer = window.setTimeout(fade, 50);
        }
    })();
}

// hide everything except for the first genome and its output
inputElements[1].style.display = 'none';
inputElements[2].style.display = 'none';
outputContainers[1].style.display = 'none';
outputContainers[2].style.display = 'none';
genomeButtons[0].style['background-color'] = '#2fd637';
genomeButtons[0].style.color = 'white';
alertBox.style.opacity = 0;
let curGenome = 0;
let helpBoxOpen = false;

// add genome switching logic
for (let i = 0; i < genomeButtons.length; i++) {
    genomeButtons[i].addEventListener('click', function(e) {
        curGenome = i;
        inputElements[i].style.display = null;
        outputContainers[i].style.display = null;
        genomeButtons[i].style['background-color'] = '#2fd637';
        genomeButtons[i].style.color = 'white';

        for (let j = 0; j < inputElements.length; j++) {
            if (i !== j) {
                inputElements[j].style.display = 'none';
                outputContainers[j].style.display = 'none';
                genomeButtons[j].style['background-color'] = null;
                genomeButtons[j].style.color = null;
            }
        }
    });
}

// add render logic
operationButtons[0].addEventListener('click', function(e) {
    if (throttles[curGenome]) { return; }
    throttles[curGenome] = true;
    let genome = curGenome;
    makeRequest('POST', '/render', inputElements[genome].value, function(renderRes) {
        if (renderRes.substring(0, 7) !== "ERROR: ") {
            outputContainers[genome].innerHTML = renderRes;
        }
        else {
            // alert the user of the error
            makeAlert(renderRes);
            outputContainers[genome].innerHTML = "";
        }
    });
});

// add mutate logic
operationButtons[1].addEventListener('click', function(e) {
    throttles[curGenome] = false;
    let genome = curGenome;

    makeRequest('POST', '/mutate', inputElements[genome].value, function(mutateRes) {
        if (mutateRes.substring(0,7) !== "ERROR: ") {
            console.log(mutateRes);
            inputElements[genome].value = mutateRes;
            /*
            makeRequest('POST', '/render', mutateRes, function(renderRes) {
                if (renderRes.substring(0, 7) !== "ERROR: ") {
                    outputContainers[genome].innerHTML = renderRes;
                }
                else {
                    // alert the user of the error
                    makeAlert(renderRes);
                    outputContainers[genome].innerHTML = "";
                }
            });
            */
        }
        else {
            // alert the user of the error
            makeAlert(mutateRes);
        }
    });
});

// add cross logic
operationButtons[2].addEventListener('click', function(e) {
    throttles[curGenome] = false;
    let genome = curGenome;
    let parentOne = genome - 1;
    if (parentOne < 0) { parentOne = inputElements.length - 1; }
    let parentTwo = genome + 1;
    if (parentTwo >= inputElements.length) { parentTwo = 0; }

    makeRequest('POST', '/cross',
                inputElements[parentOne].value + " $ " + inputElements[parentTwo].value,
                function(crossRes) { 
                    if (crossRes.substring(0, 7) !== "ERROR: ") {
                        inputElements[genome].value = crossRes;
                        /*
                        makeRequest('POST', '/render', crossRes, function(renderRes) {
                            if (renderRes.substring(0, 7) !== "ERROR: ") {
                                outputContainers[genome].innerHTML = renderRes;
                            }
                            else {
                                // alert the user of the error
                                makeAlert(renderRes);
                                outputContainers[genome].innerHTML = "";
                            }
                        });
                        */
                    }
                    else {
                        // alert the user of the error
                        makeAlert(crossRes);
                    }
                });
});


// help menu logic
let helpTimeout;
let helpAnimation;
let helpRight = -50;

function closeHelp() {
    helpRight -= 2;
    helpContainer.style.right = helpRight + '%';
    if (helpRight > -50) { setTimeout(helpAnimation, 5); }
    else {
        helpTimeout = null;
    }
}

function openHelp() {
    helpRight += 2;
    helpContainer.style.right = helpRight + '%';
    if (helpRight < 0) { setTimeout(helpAnimation, 5); }
    else { 
        helpTimeout = null;
    }
}

function toggleHelp() {
    if (helpBoxOpen) {
        helpAnimation = closeHelp;
        if (!helpTimeout) {
            helpTimeout = setTimeout(closeHelp, 5);
        }
        helpBoxOpen = false;
    }
    else {
        helpAnimation = openHelp;
        if (!helpTimeout) {
            helpTimeout = setTimeout(openHelp, 5);
        }
        helpBoxOpen = true;
    }
}

operationButtons[3].addEventListener('click', toggleHelp);

// load example logic
let example1;
let example2;
let example3;
makeRequest('GET', '/example1.txt', null, function(res) { example1 = res; });
makeRequest('GET', '/example2.txt', null, function(res) { example2 = res; });
makeRequest('GET', '/example3.txt', null, function(res) { example3 = res; });

loadExample.addEventListener('click', function(e) {
    throttles[0] = false;
    throttles[1] = false;
    throttles[2] = false;
    inputElements[0].value = example1;
    inputElements[1].value = example2;
    inputElements[2].value = example3;
});

// render throttling
let throttles = [true, true, true];
for (let i = 0; i < inputElements.length; i++) {
    inputElements[i].addEventListener('click', function(e) {
        throttles[curGenome] = false;
    });
}
