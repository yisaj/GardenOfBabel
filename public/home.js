const inputElements = document.querySelectorAll('#inputPane textarea');
const outputContainers = document.querySelectorAll('.outputContainer');
const genomeButtons = document.querySelectorAll('#genomeSelect button');
const operationButtons = document.querySelectorAll('#operationSelect button');
const alertBox = document.querySelector('#alertBox');

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
        if (elapsed > 100) {
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
alertBox.style.opacity = 0;
let curGenome = 0;

// add genome switching logic
for (let i = 0; i < genomeButtons.length; i++) {
    genomeButtons[i].addEventListener('click', function(e) {
        curGenome = i;
        inputElements[i].style.display = null;
        outputContainers[i].style.display = null;

        for (let j = 0; j < inputElements.length; j++) {
            if (i !== j) {
                inputElements[j].style.display = 'none';
                outputContainers[j].style.display = 'none';
            }
        }
    });
}

// add render logic
operationButtons[0].addEventListener('click', function(e) {
    let genome = curGenome;
    makeRequest('POST', '/render', inputElements[genome].value, function(renderRes) {
        if (renderRes.length === 0) {
            // alert the user of the error
            makeAlert('Invalid input genome.');
        }
        outputContainers[genome].innerHTML = renderRes;
    });
});

// add mutate logic
operationButtons[1].addEventListener('click', function(e) {
    let genome = curGenome;

    makeRequest('POST', '/mutate', inputElements[genome].value, function(mutateRes) {
        if (mutateRes !== 0) {
            inputElements[genome].value = mutateRes;
            
            makeRequest('POST', '/render', mutateRes, function(renderRes) {
                if (renderRes.length === 0) {
                    // alert the user of the error
                    makeAlert('Invalid input genome.');
                }
                outputContainers[genome].innerHTML = renderRes;
            });
        }
        else {
            // alert the user of the error
            makeAlert('Invalid mutation result genome.');
        }
    });
});

// add cross logic
operationButtons[2].addEventListener('click', function(e) {
    let genome = curGenome;
    let parentOne = genome + 1;
    if (parentOne >= inputElements.length) { parentOne = 0; }
    let parentTwo = genome - 1;
    if (parentTwo < 0) { parentTwo = inputElements.length - 1; }

    makeRequest('POST', '/cross',
                inputElements[parentOne].value + " $ " + inputElements[parentTwo].value,
                function(crossRes) { 
                    if (crossRes.length !== 0) {
                        inputElements[genome].value = crossRes;
                        
                        makeRequest('POST', '/render', crossRes, function(renderRes) {
                            if (renderRes.length === 0) {
                                // alert the user of the error
                                makeAlert('Invalid child genome.');
                            }
                            outputContainers[genome].innerHTML = renderRes;
                        });
                    }
                    else {
                        // alert the user of the error
                        makeAlert('Invalid parent genome or gene number mismatch.');
                    }
                });
});
