const canvas = document.getElementById('snowCanvas');
const ctx = canvas.getContext('2d');

// To get the container height and width
const container = document.querySelector('.containerCanvas');


const snows = [];
const numParticles = 50;

const colors = [
    'rgb(173, 216, 230)', // light blue
    'rgb(224, 255, 255)', // light cyan
    'rgb(175, 238, 238)', // pale turquoise
    'rgb(240, 248, 255)', // alice blue (almost white)
    'rgb(230, 230, 250)' // lavender (light blue with a hint of purple)
];


class Snow {
    constructor(x, y, size, color) {
        this.x = x;
        this.y = y;
        this.size = size;
        this.color = color;
        // Change to / movement
        this.dx = -1 * Math.random(); // Horizontal speed to left
        this.dy = 5 * Math.random(); // Vertical speed to down
    }

    draw() {
        // Circle shape
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.size, 0, Math.PI * 2);
        ctx.fillStyle = this.color;
        ctx.fill();
    }

    update() {
        // Move particles
        this.x += this.dx;
        this.y += this.dy;

        // Change to go up the screen
        if (this.x < 0) this.x = canvas.width;
        if (this.y > canvas.height) this.y = 0;

        // Draw particle
        this.draw();
    }
}

function init() {
    snows.length = 0; // Clear the array
    for (let i = 0; i < numParticles; i++) {
        const size = Math.random() * 4 + 4;
        const x = Math.random() * canvas.width;
        // Snow from top
        const y = 0;
        // Chose random pinks from colors
        const color = colors[Math.floor(Math.random() * colors.length)];
        // Create new snow and put it in the snows
        snows.push(new Snow(x, y, size, color));
    }
}

function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    for (let i = 0; i < snows.length; i++) {
        snows[i].update();
    }
    requestAnimationFrame(animate);
}

// Resize the canvas to match the container size
function resizeCanvas() {
    canvas.width = container.clientWidth;
    canvas.height = container.clientHeight;
}

// Reinitialize particles when resizing
function handleResize() {
    resizeCanvas();
    init();
}


// Initial canvas size setup
resizeCanvas();
init();
animate();
// Add a resize event listener
window.addEventListener('resize', handleResize);
