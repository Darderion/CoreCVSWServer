
let statsLoaded = true;
let fps = 0;

let cameraImage = {
    displaying: true,
    heightChecked: false,
    initHeight : 0,
    slideToggle: function () {
        if (!this.div) {
            this.div = document.getElementById('imgDiv');
            this.div.style.height = `${this.div.offsetHeight}px`;
        }
        if (!this.button) {
            this.button = document.getElementById('menuOptionCamera');
        }
        if (!this.heightChecked) {
            this.initHeight = this.div.offsetHeight;
            this.heightChecked = true;
        }
        if (this.displaying) {
            this.button.innerHTML = 'Show camera';
            this.button.classList.add('gray');
            this.displaying = false;
            this.div.style.height = '0px';
        } else {
            this.button.innerHTML = 'Hide camera';
            this.button.classList.remove('gray');
            this.displaying = true;
            this.div.style.height = `${this.initHeight}px`;
        }
    }
}

function onLoad() {
    function recursive_Ajax(url, func) { return send_Ajax(url, func, true); }
    function send_Ajax(url, func, recursive = false) {
        const xhr = new XMLHttpRequest();
        xhr.open('GET', url, true);
        xhr.onload = function() {
            if (this.status === 200) {
                func(this.responseText);
                if (recursive) send_Ajax(url, func, true);
            }
        };
        xhr.send();
    }

    setInterval(_ => {
        if (statsLoaded) {
            statsLoaded = false;
            send_Ajax('stats_request', data => {
                const arr = data.split('$');
                document.getElementById('RollDiv').innerHTML = arr[0];
                document.getElementById('PitchDiv').innerHTML = arr[1];
                document.getElementById('YawDiv').innerHTML = arr[2];
                statsLoaded = true;
            })
        }
    }, 250);

    recursive_Ajax('image_request', data => {
        document.getElementById('img').setAttribute('src', `data:image/png;base64,${data}`);
        fps++;
    });

    setInterval(_ => { console.log(`FPS: ${fps}`); document.getElementById('FPS_Div').innerHTML = `FPS:${fps}`; fps = 0; }, 1000);
}

(function(fn){
    if (document.readyState !== 'loading') {
        fn();
    } else {
        document.addEventListener('DOMContentLoaded', fn);
    }
})(onLoad)
