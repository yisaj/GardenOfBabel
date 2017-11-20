const renderButton = document.getElementById('renderButton');
const mutateButton = document.getElementById('mutateButton');
const crossButton = document.getElementById('crossButton');
const inputOne = document.getElementById('inputOne');
const inputTwo = document.getElementById('inputTwo');
const output = document.getElementById('output');
const flowerContainer = document.getElementById('flowerContainer');

renderButton.addEventListener('click', function(e) {
    let req = new XMLHttpRequest();
    req.open('POST', '/render', true);
    req.onreadystatechange = function() {
        if (req.readyState !== 4 || req.status !== 200) { return; } // on error
        flowerContainer.innerHTML = req.responseText;
    };
    req.send(output.value);
});

mutateButton.addEventListener('click', function(e) {
    let req = new XMLHttpRequest();
    req.open('POST', '/mutate', true);
    req.onreadystatechange = function() {
        if (req.readyState !== 4 || req.status !== 200) { return; } // on error
        output.value = req.responseText;
    };
    req.send(inputOne.value);
});

crossButton.addEventListener('click', function(e) {
    let req = new XMLHttpRequest();
    req.open('POST', '/cross', true);
    req.onreadystatechange = function() {
        if (req.readyState !== 4 || req.status !== 200) { return; } // on error
        output.value = req.responseText;
    };
    req.send(inputOne.value + " $ " + inputTwo.value);
});
