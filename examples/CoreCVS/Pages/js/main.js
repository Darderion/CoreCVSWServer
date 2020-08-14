
let statsLoaded = true;
let fps = 0;
const properties = {
    values : [],
    types : [
        'property_confirmed',
        'property_loading',
        'property_failed'
    ],
    PROPERTY_CONFIRMED: 0,
    PROPERTY_LOADING: 1,
    PROPERTY_FAILED: 2,
    maxIndex : 0, // This property is equal to (values.size + 1)
    set(index, value) {
        this.values[index] = value;
        let param = document.getElementById(`property${index+1}`);
        param.value = value;
        this.types.forEach(type => param.classList.remove(type));
        param.classList.add(this.types[this.PROPERTY_CONFIRMED]);
    },
    initialize() {
        let param;
        while(param = document.getElementById(`property${++this.maxIndex}`)) {
            param.oninput = function() {
                properties.types.forEach(type => this.classList.remove(type));
                this.classList.add(properties.types[properties.PROPERTY_LOADING]);
            }
        }
    }
}

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
    properties.initialize();
    setInterval(_ => {
        if (statsLoaded) {
            statsLoaded = false;
            send_Ajax('stats_request', data => {
                const arr = data.split('@');

                const modes = arr[0].split('$');

                const parameters_changed = modes.includes('Parameters_Changed');

                const arr_stats = arr[1].split('$');
                document.getElementById('RollDiv').innerHTML = arr_stats[0];
                document.getElementById('PitchDiv').innerHTML = arr_stats[1];
                document.getElementById('YawDiv').innerHTML = arr_stats[2];

                const arr_params = arr[2].split('$');
                arr_params.forEach((el, ind) => {
                    if (// Initialize a value
                        !properties.values[ind] ||
                        // Either update a value or discard any changes made to it if parameters have been changed on a server
                        (properties.values[ind] !== el && parameters_changed)) {
                        properties.set(ind, el);
                    }
                })
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
