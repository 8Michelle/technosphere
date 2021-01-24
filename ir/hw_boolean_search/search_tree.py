class Node:
    def __init__(self, query, posting_lists):
        if "|" in query:
            query_list = query.split(" | ")
            self.left = Node(" | ".join(query_list[len(query_list) // 2:]), posting_lists)
            self.right = Node(" | ".join(query_list[:len(query_list) // 2]), posting_lists)
            self.operator = '|'

        elif "&" in query:
            query_list = query.split(" & ")
            self.left = Node(" & ".join(query_list[len(query_list) // 2:]), posting_lists)
            self.right = Node(" & ".join(query_list[:len(query_list) // 2]), posting_lists)
            self.operator = '&'

        else:
            self.term = query
            index = 1 if query[0] == '!' else 0
            self.posting_list = posting_lists[query[index:]]
            self.index = 0


    def find_docid(self, docid):
        for index, i in enumerate(self.posting_list[self.index:]):
            if i >= docid:
                self.index = index
                return i
        return -1


    def safe_evaluate(self, left, right, comp):
        if right[0] != -1 and left[0] != -1:
            return comp(right[0], left[0])
        elif right[0] != -1:
            return right[0]
        elif left[0] != -1:
            return left[0]
        else:
            return -1




    def goto(self, docid):
        if hasattr(self, "operator"):
            self.left.goto(docid)
            self.right.goto(docid)
        else:
            self.docid = self.find_docid(docid)
            self.label = docid == self.docid
            self.inv = self.term[0] == '!'


    def evaluate(self):
        if hasattr(self, "operator"):
            left, right = self.left.evaluate(), self.right.evaluate()

            if self.operator == '&':
                if right[2] and left[2]:
                    docid = self.safe_evaluate(left, right, min)
                    label = left[1] or right[1]
                    inv = True

                elif left[2]:
                    if (left[0] == right[0]):
                        docid = left[0]
                        label = False
                        inv = True
                    else:
                        docid = right[0]
                        label = right[1]
                        inv = False

                elif right[2]:
                    if (left[0] == right[0]):
                        docid = right[0]
                        label = False
                        inv = True
                    else:
                        docid = left[0]
                        label = left[1]
                        inv = False

                else:
                    docid = self.safe_evaluate(left, right, max)
                    label = left[1] and right[1]
                    inv = False

            else:
                docid = self.safe_evaluate(left, right, min)
                label = left[1] or right[1]
                inv = False

        else:
            docid = self.docid
            label = self.label
            inv = self.term[0] == '!'

        return docid, label, inv


    def __repr__(self):
        if hasattr(self, "term"):
            return self.term
        else:
            print(self.operator)
            return repr(self.left) + " " + repr(self.right)




class QueryTree:
    def __init__(self, query, posting_lists):
        self.query = query
        self.body = Node(query, posting_lists)
        if posting_lists.keys():
            self.end_docid = max([posting_list[-1] for posting_list in posting_lists.values() if posting_list])
        else:
            self.end_docid = -1


    def search_result(self):

        result = []
        docid = -1
        if self.end_docid != -1:
            while docid <= self.end_docid:
                self.body.goto(docid)
                sample = self.body.evaluate()

                if sample[0] != -1:
                    if sample[1]:
                        result.append(sample[0])
                    if docid == sample[0]:
                        docid += 1
                    else:
                        docid = sample[0]
                else:
                    break

        return result
