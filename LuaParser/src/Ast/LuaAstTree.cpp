#include "LuaParser/Ast/LuaAstTree.h"
#include "LuaParser/Parse/LuaParser.h"
#include "LuaParser/Lexer/LuaTokenTypeDetail.h"

LuaAstTree::LuaAstTree()
        : _file() {

}

void LuaAstTree::BuildTree(LuaParser &p) {
    _file = p.GetLuaFile();
    StartNode(LuaNodeType::File, p);

    auto &events = p.GetEvents();
    std::vector<LuaNodeType> parents;
    for (auto i = 0ull; i != events.size(); i++) {
        switch (events[i].Type) {
            case MarkEventType::NodeStart: {
                auto e = events[i];
                if (e.U.Start.Kind == LuaNodeType::None) {
                    continue;
                }

                parents.push_back(e.U.Start.Kind);
                auto parentPos = e.U.Start.Parent;
                while (parentPos > 0) {
                    if (events[parentPos].Type == MarkEventType::NodeStart) {
                        auto &pe = events[parentPos];
                        parents.push_back(pe.U.Start.Kind);
                        parentPos = pe.U.Start.Parent;
                        pe.U.Start.Kind = LuaNodeType::None;
                    } else {
                        break;
                    }
                }
                for (auto rIt = parents.rbegin(); rIt != parents.rend(); rIt++) {
                    StartNode(*rIt, p);
                }
                parents.clear();
            }
            case MarkEventType::EatToken: {
                EatComments(p);
                EatToken(p);
            }
            case MarkEventType::NodeEnd: {
                FinishNode(p);
            }
        }
    }

    FinishNode(p);
}

void LuaAstTree::StartNode(LuaNodeType kind, LuaParser &p) {
    if (kind != LuaNodeType::Block) {
        EatComments(p);
        BuildNode(kind);
    } else {
        BuildNode(kind);
    }
}

void LuaAstTree::EatComments(LuaParser &p) {
    auto &tokens = p.GetTokens();
    for (; _tokenIndex < tokens.size(); _tokenIndex++) {
        switch (tokens[_tokenIndex].TokenType) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                EatToken(p);
                break;
            }
            default: {
                return;
            }
        }
    }
}

void LuaAstTree::EatToken(LuaParser &p) {
    auto &token = p.GetTokens()[_tokenIndex];
    _tokenIndex++;
    BuildToken(token);
}

void LuaAstTree::FinishNode(LuaParser &p) {
    if (!_nodePosStack.empty()) {
        auto nodePos = _nodePosStack.top();
        auto &node = _nodes[nodePos];
        if (node.Type == NodeOrTokenType::Node && node.Data.NodeKind == LuaNodeType::Block) {
            EatComments(p);
        } else {
            if (_tokenIndex < p.GetTokens().size() && _tokenIndex > 0) {
                auto nextToken = p.GetTokens()[_tokenIndex];
                switch (nextToken.TokenType) {
                    case TK_SHORT_COMMENT:
                    case TK_LONG_COMMENT: {
                        auto nextLine = _file->GetLine(nextToken.Range.StartOffset);
                        auto currentLine = _file->GetLine(p.GetTokens()[_tokenIndex - 1].Range.EndOffset);
                        // inline comment
                        if (currentLine == nextLine) {
                            EatToken(p);
                        }
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }

        _nodePosStack.pop();
    }
}

void LuaAstTree::BuildNode(LuaNodeType kind) {
    auto currentPos = _nodes.size();
    auto &currentNode = _nodes.emplace_back(kind);
    if (!_nodePosStack.empty()) {
        auto parentIndex = _nodePosStack.top();
        auto &parentNode = _nodes[parentIndex];
        if (parentNode.FirstChild == 0) {
            parentNode.FirstChild = currentPos;
            parentNode.LastChild = currentPos;
        } else {
            auto &lastNode = _nodes[parentNode.LastChild];
            lastNode.Sibling = currentPos;
            parentNode.LastChild = currentPos;
        }
    }
    _nodePosStack.push(currentPos);
}

void LuaAstTree::BuildToken(LuaToken &token) {
    auto currentPos = _nodes.size();
    auto &currentNode = _nodes.emplace_back(token);
    if (!_nodePosStack.empty()) {
        auto parentIndex = _nodePosStack.top();
        auto &parentNode = _nodes[parentIndex];
        if (parentNode.FirstChild == 0) {
            parentNode.FirstChild = currentPos;
            parentNode.LastChild = currentPos;

        } else {
            auto &lastNode = _nodes[parentNode.LastChild];
            lastNode.Sibling = currentPos;
            parentNode.LastChild = currentPos;
        }
        currentNode.Parent = parentIndex;
    }
}

const LuaFile &LuaAstTree::GetFile() const {
    return *_file;
}

std::size_t LuaAstTree::GetStartOffset(std::size_t index) const {
    return 0;
}

std::size_t LuaAstTree::GetEndOffset(std::size_t index) const {
    return 0;
}

std::size_t LuaAstTree::GetSibling(std::size_t index) const {
    return 0;
}

std::size_t LuaAstTree::GetFirstChild(std::size_t index) const {
    return 0;
}

LuaNodeType LuaAstTree::GetNodeType(std::size_t index) const {
    return LuaNodeType::ExpressionStatement;
}

LuaTokenType LuaAstTree::GetTokenType(std::size_t index) const {
    return 0;
}

bool LuaAstTree::IsNode(std::size_t index) const {
    return false;
}

std::size_t LuaAstTree::GetParent(std::size_t index) const {
    return 0;
}

