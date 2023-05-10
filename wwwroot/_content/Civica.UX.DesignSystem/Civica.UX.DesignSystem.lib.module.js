export function BlazorScrollToId(id) {
    const element = document.getElementById(id);
    if (element instanceof HTMLElement) {
        element.focus();
        element.scrollIntoView({
            behavior: "smooth",
            block: "start",
            inline: "nearest"
        });
    }
}

export function CopyToClipboard(codeElement) {
    navigator.clipboard.writeText(codeElement.textContent);
}

export function ClickFileUpload(id) {
    document.getElementById(id).click();
}
