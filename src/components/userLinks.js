import React from 'react'
import footerStyles from './modules/footer.module.scss'
const UserLink = (props)=>{
    function capitalize(string) {
        return string.charAt(0).toUpperCase() + string.slice(1);
    }
    const [ github,] = props.link
    return(
        <div>
            <a href={`https://github.com/${github.username}`} target="_blank" rel="noopener noreferrer" className={footerStyles.link} ><span>{capitalize(github.label)}</span></a>
        </div>
    )
}
export default UserLink
