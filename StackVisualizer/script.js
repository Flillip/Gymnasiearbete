const adderValue = document.getElementById("val");
const adderBtn = document.getElementById("addrBtn");
const popperValue = document.getElementById("pop");
const modAddr = document.getElementById("mod");
const modValue = document.getElementById("modVal");
const modBtn = document.getElementById("modBtn");
const readAddr = document.getElementById("read");
const readValue = document.getElementById("readVal");
const readBtn = document.getElementById("readBtn");
const stack = document.getElementById("stack");
const pseudoAsm = document.getElementById("asm");
const movValue = document.getElementById("mov");
const spdiv = document.getElementById("sp");
const bpdiv = document.getElementById("bp");

adderValue.value = "";
modAddr.value = "";
modValue.value = "";
readAddr.value = "";
readValue.value = "";
popperValue.value = "";

class Stack {
    #stack = 0;
    #base = 0;
    #stackArrPtr = 0;
    #baseArrPtr = 0;
    #arr = [];


    get stackArrayPointer() {
        return stack.children.length - this.#stackArrPtr - 1;
    }

    set stackArrayPointer(value) {
        this.#stackArrPtr = value;
        this.#stack = isEven(this.#stack) ? value * 2 : (value * 2) + 1;
        this.#arrUpdate(this.#stackArrPtr);
    }

    get baseArrayPointer() {
        return stack.children.length - this.#baseArrPtr - 1;
    }

    set baseArrayPointer(value) {
        this.#baseArrPtr = value;
        this.#base = isEven(this.#base) ? value * 2 : (value * 2) + 1;
        this.#arrUpdate(this.#baseArrPtr);
    }

    get stackPointer() {
        return this.#stack;
    }

    set stackPointer(value) {
        const prevSelected = stack.children[this.stackArrayPointer];
        if (prevSelected !== undefined) {
            prevSelected.classList.remove("spl");
            prevSelected.classList.remove("sph");
        }
        
        this.#stack = value;
        value *= -1;
        const uneven = value < 0 ? value + 1 : value - 1;
        const newVal = isEven(value) ? value / 2 : uneven / 2;
        this.#stackArrPtr = newVal;
        this.#arrUpdate(newVal);
        stack.children[this.stackArrayPointer].classList.add(isEven(value) ? "spl" : "sph");
    }

    get basePointer() {
        return this.#base;
    }

    set basePointer(value) {
        const prevSelected = stack.children[this.baseArrayPointer];
        if (prevSelected !== undefined) {
            prevSelected.classList.remove("bpl");
            prevSelected.classList.remove("bph");
        }
        
        this.#base = value;
        value *= -1;
        const uneven = value < 0 ? value + 1 : value - 1;
        const newVal = isEven(value) ? value / 2 : uneven / 2;
        this.#baseArrPtr = newVal;
        this.#arrUpdate(newVal);
        stack.children[this.baseArrayPointer].classList.add("bpl");
    }

    #arrUpdate(ptr, sp = true) {
        if (ptr >= this.#arr.length) {
            const amount = ptr - this.#arr.length;
            for (let i = 0; i < amount; i++) {
                this.#arr.push('');
            }
        } else if (ptr < 0) {
            const amount = 0 - ptr;
            for (let i = 0; i < amount; i++) {
                this.#arr.unshift('');
                stack.appendChild(newCell('...'));
            }
        }
    }

    updateBasePointer() {
        this.#base += 0;
    }

    push(value) {
        this.#stackArrPtr += 2;
        this.#arr[this.#stackArrPtr] = value;
    }
}

let pointers = new Stack();
let currentElementWithBp = undefined;
let currentElementWithSp = undefined;

pseudoAsm.innerHTML += "; setup stack<br/>";
pseudoAsm.innerHTML += "mov bp 0x8000<br/>";
pseudoAsm.innerHTML += "mov sp, bp<br/>";
pseudoAsm.innerHTML += "----<br/>";

function newCell(value) {    
    const newCell = document.createElement("td");
    newCell.innerText = isNaN(value) ? value : numToHex(value);
    newCell.classList.add("animate");
    return newCell;
}

function addValue() {
    let value = adderValue.value;
    
    if (value === '') {
        value = '...';
        console.log("asd")
    }

    insert(value);
    adderValue.value = '';
}

function popValue() {
    remove()
}

function callFunc() {
}

function retFunc() {

}

function insert(value, skipLog = false) {
    const cell = newCell(value);    

    const index = pointers.stackArrayPointer; // TODO: fix this
    const elementBellow = stack.childNodes[index];
    stack.insertBefore(cell, elementBellow);
    pointers.stackPointer -= 2;
    pointers.updateBasePointer();
}

function remove(skipLog = false, callback = () => { }) {
    stack.removeChild(stack.children[pointers.stackArrayPointer]);
    pointers.stackPointer += 2;
}

function modify() {

}

function read() {

}

function pusha() {

}

function popa() {

}

function movbp() {

}

function movsp() {

}

function isEven(num) {
    return num % 2 === 0;
}

function isOdd(num) {
    return !isEven(num);
}

// function getIndex(pointer) {
//     return isEven(pointer) ?
//         (pointer + 2) / 2 :
//         (pointer + 1) / 2 // REMINDER: tweak this num if something goes wrong
// }

// function numToHex(num) {
//     const prefix = num < 0 ? "-0x" : "0x";
//     num *= num < 0 ? -1 : 1;
//     return prefix + parseInt(num, 10).toString(16);
// }

function numToHex(num) {
    const prefix = num < 0 ? "-0x" : "0x";
    num *= num < 0 ? -1 : 1;

    const hexString = parseInt(num, 10).toString(16);

    // Check if the length of the hexString is odd, and if so, pad it with a leading '0'
    const paddedHexString = hexString.length % 2 === 0 ? hexString : '0' + hexString;

    // Ensure that the result consists of two 8-bit bytes
    const byte1 = paddedHexString.substring(2, 4).padStart(2, '0');
    const byte2 = paddedHexString.substring(0, 2).padStart(2, '0');

    return prefix + byte1 + ' ' + prefix + byte2;
}

function enterBtn(input, btn) {
    input.addEventListener("keypress", (e) => {
        if (e.key === "Enter") {
            e.preventDefault();
            btn.click();
        }
    });
}

enterBtn(adderValue, adderBtn);
enterBtn(modAddr, modBtn);
enterBtn(modValue, modBtn);
enterBtn(readAddr, readBtn);

const cell = newCell('...');
cell.classList.add("bpl", "spl");
stack.appendChild(cell);