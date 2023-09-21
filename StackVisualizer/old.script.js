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

let stackPointer = 0;
let basePointer = 0;
let currentElementWithBp = undefined;
let currentElementWithSp = undefined;

pseudoAsm.innerHTML += "; setup stack<br/>";
pseudoAsm.innerHTML += "mov bp 0x8000<br/>";
pseudoAsm.innerHTML += "mov sp, bp<br/>";
pseudoAsm.innerHTML += "----<br/>";

function addValue() {
    if (adderValue.value === "") {
        alert("adder value is empty");
        return;
    }

    if (isNaN(adderValue.value)) {
        alert("Value is NaN");
        return;
    }

    if (parseInt(adderValue.value) > 0xFFFF) {
        alert("Value to big for 16 bit stack");
        return;
    }

    insert(adderValue.value);
    adderValue.value = "";
}

function callFunc() {
    insert("function call", true);
}

function retFunc() {
    if (stack.childElementCount === 0) {
        alert("stack is empty");
        return;
    }

    else if (stack.firstChild.innerText !== "function call") {
        alert("not a function");
        return;
    }

    pop();
}

function insert(value, isFunc = false, skipLog = false, callback = () => { }, exact = false, increasePointer = true) {
    let hexNum = numToHex(value);
    if (hexNum.length > 2) {
        hexNum = hexNum === 3 ? "0" + hexNum : hexNum;
        hexNum = hexNum
            .match(/.{1,2}/g)
            .map((val) => '0x' + val.padStart("2", "0"))
            .toString()
            .replace(",", " ");
    } else {
        hexNum = '0x00 0x' + hexNum.padStart("2", "0");
    }

    const newCell = document.createElement("td");
    newCell.innerText = isFunc ? value : hexNum;
    newCell.classList.add("animate");
    newCell.addEventListener("animationend", callback, { once: true });
    const index =
        stack.children.length - Math.floor(stackPointer / 2) - 1;
    stack.insertBefore(newCell, stack.children[index] || stack.firstChild);
    if (!skipLog) {
        pseudoAsm.innerHTML += (isFunc && !exact ? "call func" : exact ? "push " + value : "push " + hexNum.replace(" 0x", "")) + "<br/>";
        pseudoAsm.innerHTML += "----<br/>";
    }

    if (increasePointer) {
        stackPointer += 2;
        refreshPointers();
        refreshPointers(true);
    }

}

function pop(skipLog = false, callback = () => { }) {
    const index =
        stack.children.length - Math.floor(stackPointer / 2) - 1;
    if (stack.childElementCount === 0 || stack.children[index] === undefined) {
        stackPointer -= 2;
        refreshPointers(true);
        if (!skipLog) {
            pseudoAsm.innerHTML += "pop non existent <br/>";
            pseudoAsm.innerHTML += "----<br/>";
        }
        callback();
        return;
    }

    const first = stack.children[index];
    const val = first.innerText;
    first.classList.remove('animate');
    first.style.animation = 'none'; // clear animation
    first.offsetHeight; /* trigger reflow */
    first.classList.add("deanimate");
    first.addEventListener("animationend", (_) => { stack.removeChild(first); callback() })
    stackPointer -= 2;
    refreshPointers(true);

    popperValue.value = stack.children[index].innerText.replace(" 0x", "");
    if (!skipLog) {
        pseudoAsm.innerHTML += val === "function call" ? "ret <br/>" : "pop register <br/>";
        pseudoAsm.innerHTML += "----<br/>";
    }
}

function modify() {
    if (stack.children.length === 0) {
        alert("stack is empty");
        return;
    }

    const value = parseInt(modAddr.value);

    if (isNaN(value)) {
        alert("Value is NaN");
        return;
    } else if (value < 0) {
        alert("Value has to be positive");
        return;
    }

    const stackItem = (value % 2 !== 0) ? value - 1 : value;

    if (stack.children.length <= stackItem / 2) {
        alert("Too big off an address");
        return;
    }

    const td = stack.children[stackItem / 2];
    const text = td.innerText.split(" ");
    text[(value + 1) % 2] = numToHex(modValue.value);

    pseudoAsm.innerHTML += "push bp <br/>";
    pseudoAsm.innerHTML += "mov bp, sp <br/>";
    pseudoAsm.innerHTML += "mov [bp+" + value + "], " + numToHex(modValue.value) + "<br/>";
    insert("bp", true, true, () => {
        pop(true);
        td.innerText = text.toString().replace(",", " ");
        pseudoAsm.innerHTML += "pop bp <br/>";
        pseudoAsm.innerHTML += "----<br/>";
    });
}

function read() {
    if (stack.children.length === 0) {
        alert("stack is empty");
        return;
    }

    const value = parseInt(readAddr.value);

    if (isNaN(value)) {
        alert("Value is NaN");
        return;
    } else if (value < 0) {
        alert("Value has to be positive");
        return;
    }

    const stackItem = (value % 2 !== 0) ? value - 1 : value;

    if (stack.children.length <= stackItem / 2) {
        alert("Too big off an address");
        return;
    }

    const td = stack.children[stackItem / 2];
    const text = td.innerText.split(" ");

    readValue.value = text[(value + 1) % 2];

    pseudoAsm.innerHTML += "push bp <br/>";
    pseudoAsm.innerHTML += "mov bp, sp <br/>";
    pseudoAsm.innerHTML += "mov register, [bp+" + value + "] <br/>";
    insert("bp", true, true, () => {
        pop(true);
        pseudoAsm.innerHTML += "pop bp <br/>";
        pseudoAsm.innerHTML += "----<br/>";
    });
}

function pusha() {
    insert("DI", true, true);
    insert("SI", true, true);
    insert("BP", true, true);
    insert("SP", true, true);
    insert("BX", true, true);
    insert("DX", true, true);
    insert("CX", true, true);
    insert("AX", true, true);
    pseudoAsm.innerHTML += "pusha <br/>";
    pseudoAsm.innerHTML += "----<br/>";
}

function popa() {
    let index = 0;
    const popaCallback = () => {
        index++;
        console.log(index)
        if (index >= 8) {
            return;
        }

        pop(true, popaCallback);
    }
    pop(true, popaCallback);
    pseudoAsm.innerHTML += "popa <br/>";
    pseudoAsm.innerHTML += "----<br/>";
}

function movbp() {
    basePointer -= parseInt(movValue.value)
    refreshPointers()
    pseudoAsm.innerHTML += "add bp, " + numToHex(movValue.value) + "<br/>";
    pseudoAsm.innerHTML += "----<br/>";
}

function movsp() {
    stackPointer -= parseInt(movValue.value)
    refreshPointers(true)
    pseudoAsm.innerHTML += "add sp, " + numToHex(movValue.value) + "<br/>";
    pseudoAsm.innerHTML += "----<br/>";
}

function refreshPointers(sp = false, remove = true) {
    const remover = remove ? 0 : 2;
    const pointer = (sp ? stackPointer : basePointer) + remover;
    const index =
        stack.children.length - Math.floor(pointer / 2) - 1;

    let currentEl = sp ? currentElementWithSp : currentElementWithBp
    if (currentEl !== undefined && remove) {
        currentEl.classList.remove(sp ? "spl" : "bpl");
    }

    if (stack.children[index] !== undefined) {
        stack.children[index].classList.add(sp ? "spl" : "bpl");
        currentEl = stack.children[index];
    } else {
        insert("empty", true, true, undefined, true, false);
        refreshPointers(sp, false)
    }

    sp ? currentElementWithSp = currentEl : currentElementWithBp = currentEl;
    sp ? 
    spdiv.innerText = "Stack pointer = " + (stackPointer) : 
    bpdiv.innerText = "Base pointer = " + (basePointer);
}

function numToHex(num) {
    const prefix = num < 0 ? "-0x" : "0x";
    num *= num < 0 ? -1 : 1;
    return prefix + parseInt(num, 10).toString(16);
}

function enterBtn(input, btn) {
    input.addEventListener("keypress", (e) => {
        if (e.key === "Enter") {
            e.preventDefault();
            btn.click();
        }
    });
}

refreshPointers()
refreshPointers(true)

enterBtn(adderValue, adderBtn);
enterBtn(modAddr, modBtn);
enterBtn(modValue, modBtn);
enterBtn(readAddr, readBtn);
